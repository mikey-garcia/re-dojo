#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PORT 4444

static void win(void)
{
    FILE *f = fopen("/flag", "r");

    if (!f) {
        puts("flag missing");
        return;
    }

    char buf[256];

    if (fgets(buf, sizeof(buf), f))
        printf("%s", buf);

    fclose(f);
}

typedef struct Address {
    int apartment;
    int floor;
} Address;

typedef struct Person {
    char *name;
    int age;
    Address *address;
    int *favorite_number;
} Person;

typedef struct ResultPacket {
    int alpha;
    int bravo;
    int charlie;
    int delta;
    int echo;
    int foxtrot;
} ResultPacket;

static void say_hello(char *name) {
    printf("Hello %s\n", name);
}

static int double_value(int x) {
    return x * 2;
}

static int add_favorite_number(Person *p) {
    return p->age + *p->favorite_number;
}

static int unused_math_function(int x) {
    int a = x + 3;
    int b = a * 2;
    int c = b - 4;
    return c;
}

void practice(int a, int b) {
    int lucky = 7;

    Address addr;
    addr.apartment = 42;
    addr.floor = 5;

    Person student;
    student.name = "Alice";
    student.age = a;
    student.address = &addr;
    student.favorite_number = &lucky;

    say_hello(student.name);

    int value1 = double_value(student.age);
    int value2 = add_favorite_number(&student);

    printf("practice values: %d %d\n", value1, value2);
    printf("apartment=%d floor=%d\n",
           student.address->apartment,
           student.address->floor);

    if (b == 1234) {
        printf("secret branch reached\n");
    }

    if (unused_math_function(10) == 9999) {
        printf("impossible branch\n");
    }
}

int main(void) {
    practice(21, 0);

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(listener);
        return 1;
    }

    if (listen(listener, 1) < 0) {
        perror("listen");
        close(listener);
        return 1;
    }

    // spawn client for ease of pwncollege dojo
    const char *client_path;

    if (access("/challenge/client", X_OK) == 0) {
        client_path = "/challenge/client";
    } else {
        client_path = "./client";
    }

    pid_t pid = fork();

    if (pid == 0) {
        execl(client_path, client_path, NULL);

        perror("execl");
        exit(1);
    } 
    // end of spawn

    printf("\nWaiting for client...\n");

    int client = accept(listener, NULL, NULL);
    if (client < 0) {
        perror("accept");
        close(listener);
        return 1;
    }

    int seed = 37;
    printf("Seed = %d\n", seed);

    send(client, &seed, sizeof(seed), 0);

    ResultPacket packet;

    ssize_t received =
    recv(client,
         &packet,
         sizeof(packet),
         0);

    if (received != sizeof(packet)) {
        printf("Failed to receive packet.\n");

        close(client);
        close(listener);

        return 1;
    }
    waitpid(pid, NULL, 0);

    printf("Client sent six values.\n");
    printf("Enter six values: ");

    int a, b, c, d, e, f;
    scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f);

    if (a == packet.alpha &&
        b == packet.bravo &&
        c == packet.charlie &&
        d == packet.delta &&
        e == packet.echo &&
        f == packet.foxtrot) {
        printf("Correct!\n");
        win();
    } else {
        printf("Incorrect.\n");
    }

    close(client);
    close(listener);

    return 0;
}