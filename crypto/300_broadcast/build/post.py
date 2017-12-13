# -*- coding:utf-8 -*-
import enum
import pickle
import requests
import datetime
import binascii

from utils import get_mac


class PostType(enum.Enum):
    URL = 1
    TEXT = 2
    CREDENTIAL = 3


class Post(object):
    def __init__(self, token, idx, content, mac):
        now = int(datetime.datetime.utcnow().timestamp())
        self.token = token
        self.idx = idx
        self.content = content
        self.mac = mac
        self.created_at = now
        self.updated_at = now

    @classmethod
    def get_by_idx(cls, posts, idx):
        try:
            return next(filter(lambda post: post.idx == idx, posts), None)
        except IndexError:
            return None

    @classmethod
    def list(cls, posts):
        return b'\n'.join(
            b'Post-%d|%s|%s' % (
                post.idx,
                str(post.created_at).encode(),
                str(post.updated_at).encode()
            ) for post in posts,
        )

    @classmethod
    def write(cls, token, type_, idx, content):
        if type_ == PostType.URL.value:
            content = Post.get_from_url(content)

        if content is None:
            return None

        key = token.replace(b'-', b'')
        mac = get_mac(key, content)
        content += b'\n\nMAC=' + binascii.hexlify(mac)
        content = pickle.dumps(content)
        return cls(token, idx, content, mac)

    @classmethod
    def read(cls, posts, idx):
        post = Post.get_by_idx(posts, idx)
        return post.content if post is not None else None

    @classmethod
    def update(cls, posts, idx, type_, content):
        post = Post.get_by_idx(posts, idx)
        if post is None:
            return None

        if type_ == PostType.URL:
            content = Post.get_from_url(content)

        if content is None:
            return None

        post.content = pickle.dumps(content)
        post.updated_at = int(datetime.datetime.utcnow().timestamp())
        return post

    @classmethod
    def delete(cls, posts, idx):
        post = Post.get_by_idx(posts, idx)
        if post is not None:
            posts.remove(post)
            del post
            return True
        return None

    @classmethod
    def is_valid_type(cls, _type):
        return _type == PostType.URL.value or _type == PostType.TEXT.value

    @staticmethod
    def get_from_url(url):
        try:
            r = requests.get(url, timeout=5)
        except requests.exceptions.Timeout:
            return None
        except UnicodeDecodeError:
            return None
        return r.content[:512] if r.status_code == 200 else None
