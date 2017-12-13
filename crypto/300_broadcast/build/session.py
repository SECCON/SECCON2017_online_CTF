# -*- coding:utf-8 -*-


class Session(object):

    def __init__(self):
        self.table = {}

    def __setitem__(self, key, value):
        self.table[key] = value

    def __getitem__(self, key):
        return self.table.get(key, None)

    def __contains__(self, key):
        return self.table[key] is not None

    def __getattr__(self, key):
        return self.table.get(key, None)

    @classmethod
    def create_sub_session(cls):
        session = cls()
        session.table['posts'] = []
        return session

    def get_posts(self):
        return self.table['posts']

    def add_post(self, post):
        self.table['posts'].append(post)
