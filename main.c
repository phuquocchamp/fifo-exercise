#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "/tmp/myfifo"
#define SIZE 7

void sender_process()
{
    int fd;
    int numbers[SIZE];

    // Tạo 7 số ngẫu nhiên
    for (int i = 0; i < SIZE; i++)
    {
        numbers[i] = rand() % 100; // Số ngẫu nhiên từ 0 đến 99
    }

    // Mở FIFO để ghi dữ liệu
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1)
    {
        perror("Không thể mở FIFO để ghi");
        exit(EXIT_FAILURE);
    }

    // Ghi dữ liệu vào FIFO
    write(fd, numbers, sizeof(numbers));
    printf("Process A đã gửi: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    // Đóng FIFO
    close(fd);
}

void receiver_process()
{
    int fd;
    int numbers[SIZE];
    int sum = 0;

    // Mở FIFO để đọc dữ liệu
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1)
    {
        perror("Không thể mở FIFO để đọc");
        exit(EXIT_FAILURE);
    }

    // Đọc dữ liệu từ FIFO
    read(fd, numbers, sizeof(numbers));
    printf("Process B đã nhận: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", numbers[i]);
        sum += numbers[i];
    }
    printf("\nTổng các số nhận được là: %d\n", sum);

    // Đóng FIFO
    close(fd);
}

int main()
{
    // Tạo FIFO nếu nó chưa tồn tại
    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        perror("Không thể tạo FIFO");
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Không thể tạo tiến trình con");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Tiến trình con - Nhận dữ liệu
        receiver_process();
    }
    else
    {
        // Tiến trình cha - Gửi dữ liệu
        sender_process();
    }

    // Xóa FIFO sau khi sử dụng
    unlink(FIFO_NAME);

    return 0;
}
