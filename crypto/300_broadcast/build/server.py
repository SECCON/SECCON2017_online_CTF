# -*- coding:utf-8 -*-
import asyncio
import datetime
from uuid import uuid4
from binascii import hexlify

from session import Session
from post import Post
from utils import get_mac

sessions = Session()
admin_token = str(uuid4()).encode()
notice_post = None
challenge_mac = None

notice_content = b"""This system is only for valid users who have contracted with our company.
If you continue to try to connect to this server without valid authorization, 
it will be blocked permanently.
Thank you."""
now = int(datetime.datetime.utcnow().timestamp())


def gen_new_token():
    while True:
        new_token = str(uuid4())
        if new_token != admin_token:
            break
    return new_token.encode()


def init():
    global notice_post
    global challenge_mac

    # init admin session
    admin_session = Session.create_sub_session()
    sessions[admin_token] = admin_session

    # add admin notice
    key = admin_token.replace(b'-', b'')
    challenge_mac = get_mac(key, notice_content)
    notice_post = Post.write(
        token=admin_token,
        type_=0,
        idx=0,
        content=notice_content,
    )
    sessions[admin_token].add_post(notice_post)

    # add flag post
    flag = b'SECCON{8e62f74287c22108e82e18b69caa115ceb8fceec532fae97971b7b01912fa313}'
    flag_post = Post.write(
        token=admin_token,
        type_=0,
        idx=1,
        content=flag,
    )
    admin_session.add_post(flag_post)


def get_admin_token(message):
    global admin_token
    global challenge_mac

    if message == notice_content:
        return None

    key = admin_token.replace(b'-', b'')
    mac = get_mac(key, message)
    if mac != challenge_mac:
        return None

    return admin_token


procedures = {
    0x00: Post.list,
    0x01: Post.write,
    0x02: Post.read,
    0x03: Post.update,
    0x04: Post.delete,
    0xff: get_admin_token,
}


class RPCServer(asyncio.Protocol):
    def __init__(self):
        self.transport = None
        self.token = None

    def send_msg(self, message):
        self.transport.write(message.encode())

    def connection_made(self, transport):
        token = gen_new_token()
        sessions[token] = Session.create_sub_session()
        self.transport = transport
        self.send_msg('Token: %s' % token.decode())
        self.token = token

    def validate_data(self, data):
        if len(data) < 37:
            self.send_msg('Invalid header length.')
            return False
        return True

    def parse_header(self, header):
        token, procedure_number, data = header[:36], header[36], header[37:]
        session = sessions[token]

        if session is None:
            self.send_msg("Invalid token.")
            return None

        if procedure_number not in procedures:
            self.send_msg('Invalid procedure number.')
            return None

        data = data.strip()
        if not data:
            self.send_msg('Empty data.')
            return None

        return token, procedure_number, data

    def handle_procedure(self, token, procedure_number, data):
        procedure = procedures[procedure_number]
        posts = sessions[token].get_posts()
        result = None

        if token == admin_token:
            if procedure not in (Post.list, Post.read):
                self.send_msg('Invalid procedure number')
                return None

        if procedure == Post.list:
            result = b'Notice-%d|%s|%s\n' % (notice_post.idx, str(now).encode(), str(now).encode())
            result += Post.list(posts)
        elif procedure == Post.write:
            if len(posts) >= 5:
                self.send_msg('Excess in the maximum number of posts')
                return None

            type_ = data[0]
            content = data[1:]
            if not Post.is_valid_type(data[0]):
                self.send_msg('Invalid post type: %r' % type_)
                return None

            post = Post.write(token, type_, len(posts) + 2, content)
            if post is None:
                self.send_msg('Timeout to fetch %r' % content)
                return None

            sessions[token].add_post(post)
            result = b'created'
        elif procedure == Post.read:
            idx = data[0]
            if idx == 0:
                result = notice_post.content
            else:
                result = Post.read(posts, idx)
        elif procedure == Post.update:
            idx = data[0]
            type_ = data[1]
            content = data[2:]
            if not Post.is_valid_type(data[1]):
                self.send_msg('Invalid post type: %r' % type_)
                return None

            post = Post.update(posts, idx, type_, content)
            if post is None:
                self.send_msg('Not found post matches with idx:%r/type:%r' % (idx, type_))
                return None
            result = b'updated'
        elif procedure == Post.delete:
            idx = data[0]
            is_deleted = Post.delete(posts, idx)
            if not is_deleted:
                self.send_msg('Not found post matches with idx:%r' % idx)
                return None
            result = b'deleted'
        elif procedure == get_admin_token:
            result = get_admin_token(data)
            if result is None:
                self.send_msg('Failed to get admin token. The message must satisfy MAC=%s' %
                              hexlify(challenge_mac).decode())
        return result

    def data_received(self, data):
        data = data.strip()
        if not self.validate_data(data):
            self.transport.close()
            return

        result = self.parse_header(data)
        if result is None:
            self.transport.close()
            return

        rc = None

        try:
            rc = self.handle_procedure(*result)
        except IndexError:
            self.send_msg('Error occurred while parsing parameters')
        if rc is None:
            self.send_msg('Bad request')
        else:
            self.transport.write(rc)
        self.transport.close()


init()
loop = asyncio.get_event_loop()
coro = loop.create_server(RPCServer, '0.0.0.0', 8888)
server = loop.run_until_complete(coro)

# Serve requests until Ctrl+C is pressed
print('Serving on {}'.format(server.sockets[0].getsockname()))
try:
    loop.run_forever()
except KeyboardInterrupt:
    pass

# Close the server
server.close()
loop.run_until_complete(server.wait_closed())
loop.close()
