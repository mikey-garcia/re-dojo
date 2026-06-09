#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4444

typedef struct ChallengeState {
    int alpha;
    int bravo;
    int charlie;
    int delta;
    int echo;
    int foxtrot;

    int *p_one;
    int *p_two;
    int *p_three;
    int *p_four;
} ChallengeState;

typedef struct ResultPacket {
    int alpha;
    int bravo;
    int charlie;
    int delta;
    int echo;
    int foxtrot;
} ResultPacket;

static void build_state(ChallengeState *s, int seed) {
    s->alpha = seed;
    s->bravo = seed + 10;
    s->charlie = seed + 20;
    s->delta = seed + 30;
    s->echo = seed + 40;
    s->foxtrot = seed + 50;

    s->p_one = &s->alpha;
    s->p_two = &s->bravo;
    s->p_three = &s->charlie;
    s->p_four = &s->delta;

    *s->p_one += 5;
    *s->p_two += 3;

    s->p_three = &s->alpha;
    *s->p_three += 2;

    s->p_four = s->p_three;
    *s->p_four *= 2;

    s->charlie -= 4;

    s->p_two = &s->echo;
    *s->p_two += 9;

    s->foxtrot = *s->p_four;
    s->foxtrot -= 6;

    s->p_one = &s->bravo;
    *s->p_one *= 2;

    s->delta += 1;
}

static ResultPacket make_packet(int seed) {
    ChallengeState state;

    build_state(&state, seed);

    ResultPacket packet;
    packet.alpha = state.alpha;
    packet.bravo = state.bravo;
    packet.charlie = state.charlie;
    packet.delta = state.delta;
    packet.echo = state.echo;
    packet.foxtrot = state.foxtrot;

    return packet;
}

int main(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    int seed = 0;
    recv(sock, &seed, sizeof(seed), 0);

    ResultPacket packet = make_packet(seed);

    send(sock, &packet, sizeof(packet), 0);

    close(sock);

    return 0;
}