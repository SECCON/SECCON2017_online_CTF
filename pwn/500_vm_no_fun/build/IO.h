#ifndef INCEPTION_IO_H
#define INCEPTION_IO_H

size_t readAll(int fd, void *buf, size_t max_len);

void readAll_vm1(void *buf, size_t len);
void readAll_vm2(void *buf, size_t len);
void readAll_vm3(void *buf, size_t len);

void write_vm1(void *buf, size_t len);
void write_vm2(void *buf, size_t len);
void write_vm3(void *buf, size_t len);

#endif //INCEPTION_IO_H
