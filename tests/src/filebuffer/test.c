#include <test.h>
#include <filebuffer.h>
#include <common.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/* 1MB buffer */
#define BUFFER_SIZE BIT(20)

const char const *test_dir = "./files";

___before_main___(1) void test_init(void)
{
    struct stat st;

    if (stat(test_dir, &st) == -1)
        mkdir(test_dir, 0775);
}

___after_main___(1) void test_deinit(void)
{
    rmdir(test_dir);
}

int write_to_file(int fd, size_t bytes, off_t offset)
{
    char buffer[BUFFER_SIZE];
    int i;
    int loops;

    srand(time(NULL));

    if (lseek(fd, offset, SEEK_SET) == (off_t)-1)
        return 1;

    /* rand */
    for (i = 0; i < BUFFER_SIZE; ++i)
        buffer[i] = rand() % ('Z' - 'A' + 1) + 'A';

    /* full loops */
    loops = bytes / BUFFER_SIZE;
    for (i = 0; i < loops; ++i)
        if (write(fd, buffer, BUFFER_SIZE) == -1)
            return 1;

    /* write last part */
    if (bytes % BUFFER_SIZE)
        if (write(fd, buffer, bytes % BUFFER_SIZE) == -1)
            return 1;

    return 0;
}

int file_copy(int fd1, int fd2)
{
    struct stat st;

    size_t file_size;
    size_t bytes;

    char buffer[BUFFER_SIZE];

    int i;
    int loops;

    memset(buffer, 0, BUFFER_SIZE);

    if (lseek(fd1, 0, SEEK_SET) == (off_t)-1)
        return 1;

    if (lseek(fd2, 0, SEEK_SET) == (off_t)-1)
        return 1;

    /* read file size */
    if (fstat(fd1, &st) == -1)
        return 1;

    file_size = (size_t)st.st_size;

    /* full loop */
    loops = file_size / BUFFER_SIZE;
    for (i = 0; i < loops; ++i)
    {
        if (read(fd1, buffer, BUFFER_SIZE) == -1)
            return 1;

        if (write(fd2, buffer, BUFFER_SIZE) == -1)
            return 1;
    }

    /* last part */
    bytes = file_size % BUFFER_SIZE;
    if (bytes)
    {
        if (read(fd1, buffer, bytes) == -1)
            return 1;

        if (write(fd2, buffer, bytes) == -1)
            return 1;
    }

    return 0;
}

int file_cmp(int fd1, int fd2)
{
    struct stat st;

    size_t file1_size;
    size_t file2_size;
    size_t bytes;

    char buffer1[BUFFER_SIZE + 1];
    char buffer2[BUFFER_SIZE + 1];

    int loops;
    int i;
    int ret;

    /* reset buffers */
    memset(buffer1, 0, BUFFER_SIZE);
    memset(buffer2, 0, BUFFER_SIZE);

    if (lseek(fd1, 0, SEEK_SET) == (off_t)-1)
        return 1;

    if (lseek(fd2, 0, SEEK_SET) == (off_t)-1)
        return 1;

    /* read file size */
    if (fstat(fd1, &st) == -1)
        return 1;

    file1_size = (size_t)st.st_size;

    if (fstat(fd2, &st) == -1)
        return 1;

    file2_size = (size_t)st.st_size;

    /* files are equal only if have equal sizes */
    if (file1_size != file2_size)
    {
        if (file1_size < file2_size)
            return -1;
        else
            return 1;
    }

    /* full loop */
    loops = file1_size / BUFFER_SIZE;
    for (i = 0; i < loops; ++i)
    {
        if (read(fd1, buffer1, BUFFER_SIZE) == -1)
            return 1;

        buffer1[BUFFER_SIZE] = '\0';

        if (read(fd2, buffer2, BUFFER_SIZE) == -1)
            return 1;

        buffer2[BUFFER_SIZE] = '\0';

        ret = strcmp(buffer1, buffer2);
        if (ret)
            return ret;
    }

    /* last part */
    bytes = file1_size % BUFFER_SIZE;
    if (bytes)
    {
        if (read(fd1, buffer1, bytes) == -1)
            return 1;

        buffer1[bytes] = '\0';

        if (read(fd2, buffer2, bytes) == -1)
            return 1;

        buffer2[bytes] = '\0';

        ret = strcmp(buffer1, buffer2);
        if (ret)
            return ret;
    }

    return 0;
}

test_f test_create_from_fd(void)
{
    char *path1;
    char *path2;
    int fd1;
    int fd2;

    /* 5 MB */
    const size_t file_size = BIT(20) * 5;

    file_buffer *fb;

    /* create file */
    T_ERROR(asprintf(&path1, "%s/%s", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path1, 0644) == -1);

    /* open file */
    fd1 = open(path1, O_RDWR);
    T_ERROR(fd1 == -1);

    /* write 5MB */
    T_ERROR(write_to_file(fd1, file_size, 0));

    /* create file */
    T_ERROR(asprintf(&path2, "%s/%s2", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path2, 0644) == -1);

    /* open file */
    fd2 = open(path2, O_RDWR);
    T_ERROR(fd2 == -1);

    /* copy file */
    T_ERROR(file_copy(fd1, fd2));

    /* TEST FILE BUFFER */
    fb = file_buffer_create(fd1, PROT_READ | PROT_WRITE);
    T_ERROR(fb == NULL);

    T_CHECK(file_buffer_get_buff(fb) != NULL);
    T_EXPECT(file_buffer_get_size(fb), file_size);

    file_buffer_destroy(fb);

    T_EXPECT(file_cmp(fd1, fd2), 0);

    close(fd1);
    close(fd2);

    T_ERROR(remove(path1) == -1);
    T_ERROR(remove(path2) == -1);

    FREE(path1);
    FREE(path2);
}

test_f test_create_from_path(void)
{
    char *path1;
    char *path2;
    int fd1;
    int fd2;

    file_buffer *fb;

    /* 5 MB */
    const size_t file_size = BIT(20) * 5;

    /* create file */
    T_ERROR(asprintf(&path1, "%s/%s", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path1, 0644) == -1);

    /* open file */
    fd1 = open(path1, O_RDWR);
    T_ERROR(fd1 == -1);

    /* write 5MB */
    T_ERROR(write_to_file(fd1, file_size, 0));

    /* create file */
    T_ERROR(asprintf(&path2, "%s/%s2", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path2, 0644) == -1);

    /* open file */
    fd2 = open(path2, O_RDWR);
    T_ERROR(fd2 == -1);

    /* copy file */
    T_ERROR(file_copy(fd1, fd2));

    /* TEST FILE BUFFER */
    fb = file_buffer_create_from_path(path1, PROT_READ | PROT_WRITE, O_RDWR);
    T_ERROR(fb == NULL);

    T_CHECK(file_buffer_get_buff(fb) != NULL);
    T_EXPECT(file_buffer_get_size(fb), file_size);

    file_buffer_destroy(fb);

    T_EXPECT(file_cmp(fd1, fd2), 0);

    close(fd1);
    close(fd2);

    T_ERROR(remove(path1) == -1);
    T_ERROR(remove(path2) == -1);

    FREE(path1);
    FREE(path2);
}

test_f test_read_file(void)
{
    char *path1;
    char *path2;
    int fd1;
    int fd2;

    const size_t file_size = BUFFER_SIZE;
    char buffer[BUFFER_SIZE + 1];

    file_buffer *fb;

    /* create file */
    T_ERROR(asprintf(&path1, "%s/%s", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path1, 0644) == -1);

    /* open file */
    fd1 = open(path1, O_RDWR);
    T_ERROR(fd1 == -1);

    /* write to file */
    T_ERROR(write_to_file(fd1, file_size, 0));

    /* come back to begining */
    T_ERROR(lseek(fd1, 0, SEEK_SET) == -1);

    /* read from file */
    T_ERROR(read(fd1, buffer, file_size) == -1);
    buffer[BUFFER_SIZE] = '\0';

    /* come back to begining */
    T_ERROR(lseek(fd1, 0, SEEK_SET) == -1);

    /* TEST FILE BUFFER */
    fb = file_buffer_create(fd1, PROT_READ | PROT_WRITE);
    T_ERROR(fb == NULL);

    T_CHECK(file_buffer_get_buff(fb) != NULL);
    T_EXPECT(file_buffer_get_size(fb), file_size);

    /* create file */
    T_ERROR(asprintf(&path2, "%s/%s2", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path2, 0644) == -1);

    /* open file */
    fd2 = open(path2, O_RDWR);
    T_ERROR(fd2 == -1);

    /* write to new file */
    T_ERROR(write(fd2, file_buffer_get_buff(fb), file_buffer_get_size(fb)) == -1);

    /* cmp files */
    T_EXPECT(file_cmp(fd1, fd2), 0);

    file_buffer_destroy(fb);

    close(fd1);
    close(fd2);

    T_ERROR(remove(path1) == -1);
    T_ERROR(remove(path2) == -1);

    FREE(path1);
    FREE(path2);
}

test_f test_write_to_file(void)
{
    char *path1;
    int fd1;

    const char *const data = "Kukos is a C fanboy!";
    const size_t file_size = 0;
    char buffer[BUFFER_SIZE];

    file_buffer *fb;

    /* create file */
    T_ERROR(asprintf(&path1, "%s/%s", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path1, 0644) == -1);

    /* open file */
    fd1 = open(path1, O_RDWR);
    T_ERROR(fd1 == -1);

    /* TEST FILE BUFFER */
    fb = file_buffer_create(fd1, PROT_READ | PROT_WRITE);
    T_ERROR(fb == NULL);

    T_CHECK(file_buffer_get_buff(fb) != NULL);
    T_EXPECT(file_buffer_get_size(fb), file_size);

    /* add data to file */
    T_EXPECT(file_buffer_append(fb, data), 0);
    T_EXPECT(file_buffer_get_size(fb), strlen(data));

    /* sync data */
    T_EXPECT(file_buffer_synch(fb), 0);

    /* read synced data */
    T_ERROR(read(fd1, buffer, strlen(data)) == -1);
    buffer[strlen(data)] = '\0';

    T_EXPECT(strcmp(data, buffer), 0);

    file_buffer_destroy(fb);

    close(fd1);
    T_ERROR(remove(path1) == -1);

    FREE(path1);
}

test_f test_read_write(void)
{
    char *path1;
    char *path2;
    int fd1;
    int fd2;

    const char *const data = "Kukos is a C fanboy!";

    /* 10 MB */
    const size_t file_size = BIT(20) * 10;

    file_buffer *fb;

    /* create file */
    T_ERROR(asprintf(&path1, "%s/%s", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path1, 0644) == -1);

    /* open file */
    fd1 = open(path1, O_RDWR);
    T_ERROR(fd1 == -1);

    /* write 10 MB to file */
    T_ERROR(write_to_file(fd1, file_size, 0));
    T_ERROR(lseek(fd1, 0, SEEK_SET) == -1);

    /* create file */
    T_ERROR(asprintf(&path2, "%s/%s2", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path2, 0644) == -1);

    /* open file */
    fd2 = open(path2, O_RDWR);
    T_ERROR(fd2 == -1);

    /* copy file */
    T_ERROR(file_copy(fd1, fd2));

    /* add data to file2 */
    T_ERROR(lseek(fd2, file_size, SEEK_SET) == -1);
    T_ERROR(write(fd2, data, strlen(data)) == -1);

    /* TEST FILE BUFFER */
    fb = file_buffer_create(fd1, PROT_READ | PROT_WRITE);
    T_ERROR(fb == NULL);

    T_CHECK(file_buffer_get_buff(fb) != NULL);
    T_EXPECT(file_buffer_get_size(fb), file_size);

    /* add data to file */
    T_EXPECT(file_buffer_append(fb, data), 0);
    T_EXPECT(file_buffer_get_size(fb), strlen(data) + file_size);

    /* sync data */
    T_EXPECT(file_buffer_synch(fb), 0);

    T_EXPECT(file_cmp(fd1, fd2), 0);

    file_buffer_destroy(fb);

    close(fd1);
    close(fd2);

    T_ERROR(remove(path1) == -1);
    T_ERROR(remove(path2) == -1);

    FREE(path1);
    FREE(path2);
}

test_f test_work_on_big_file(void)
{
    char *path1;
    char *path2;
    int fd1;
    int fd2;

    const char *const data = "Kukos is a C fanboy!";

    /* 100 MB */
    const size_t file_size = BIT(20) * 100;

    file_buffer *fb;

    /* create file */
    T_ERROR(asprintf(&path1, "%s/%s", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path1, 0644) == -1);

    /* open file */
    fd1 = open(path1, O_RDWR);
    T_ERROR(fd1 == -1);

    /* write 10 MB to file */
    T_ERROR(write_to_file(fd1, file_size, 0));
    T_ERROR(lseek(fd1, 0, SEEK_SET) == -1);

    /* create file */
    T_ERROR(asprintf(&path2, "%s/%s2", test_dir, __FUNCTION__) == -1);
    T_ERROR(creat(path2, 0644) == -1);

    /* open file */
    fd2 = open(path2, O_RDWR);
    T_ERROR(fd2 == -1);

    /* copy file */
    T_ERROR(file_copy(fd1, fd2));

    /* add data to file2 */
    T_ERROR(lseek(fd2, file_size, SEEK_SET) == -1);
    T_ERROR(write(fd2, data, strlen(data)) == -1);

    /* TEST FILE BUFFER */
    fb = file_buffer_create(fd1, PROT_READ | PROT_WRITE);
    T_ERROR(fb == NULL);

    T_CHECK(file_buffer_get_buff(fb) != NULL);
    T_EXPECT(file_buffer_get_size(fb), file_size);

    /* add data to file */
    T_EXPECT(file_buffer_append(fb, data), 0);
    T_EXPECT(file_buffer_get_size(fb), strlen(data) + file_size);

    /* sync data */
    T_EXPECT(file_buffer_synch(fb), 0);

    T_EXPECT(file_cmp(fd1, fd2), 0);

    file_buffer_destroy(fb);

    close(fd1);
    close(fd2);

    T_ERROR(remove(path1) == -1);
    T_ERROR(remove(path2) == -1);

    FREE(path1);
    FREE(path2);
}

void test(void)
{
    TEST(test_create_from_fd());
    TEST(test_create_from_path());
    TEST(test_read_file());
    TEST(test_write_to_file());
    TEST(test_read_write());
    TEST(test_work_on_big_file());
}

int main(void)
{
    TEST_INIT("FILEBUFFER");
    test();
    TEST_SUMMARY;
}
