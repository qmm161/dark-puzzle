#include <iostream>
#include <ctime>
#include <cstdlib>
#include "ts_queue.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "../../../../Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/usr/include/sys/termios.h"

using namespace std;
using namespace gdface;

// 👦👧
// 🏤🌋🗻🏠🏡🏢🏣🏤🏥🏦🏨🏩🏪🏫🏬🏭🏯🏰💒🗼🗽⛪⛲🎠🎡🎢🎪⛽🚧🗿
static char pics[] = "🏤 🌋 🗻 🏠 🏡 🏢 🏣 🏤 🏥 🏦 🏨 🏩 🏪 🏫 🏬 🏭 🏯 🏰 💒 🗼 🗽 ⛪";
static size_t pic_cnt = 0;

void init_pics()
{
    size_t len = strlen(pics);
    pic_cnt = len / strlen("🏤 ") + 1;

    for (size_t i = 0; i < len; i++)
    {
        if (pics[i] == ' ')
        {
            pics[i] = '\0';
        }
    }
}

const char *random_unit()
{
    int random = rand();
    size_t idx = random % (pic_cnt);

    const char *c = &pics[idx * (strlen("🏤 "))];
    // cout << "idx:" << idx << " " << c << endl;
    return c;
}

typedef struct _Block
{
    _Block(int i)
    {
        if (i == 0)
        {
            str = BLANK;
        }
        else
        {
            str = random_unit();
        }
    }

    size_t lenth()
    {
        return strlen(str);
    }

    bool isBlank()
    {
        return str == BLANK;
    }

    const char *str;

private:
    static char BLANK[3];
} Block;

char _Block::BLANK[3] = {' ', ' ', '\0'};

typedef enum
{
    OPER_INVALID,
    OPER_LEFT,
    OPER_RIGHT,
    OPER_UP,
    OPER_DOWN
} OPER;

threadsafe_queue<OPER> operQueue;

static void
reset_cursor()
{
    printf("%c[%d;%dH", 27, 0, 0);
}

static void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

static bool need_print(size_t line, size_t column, int pos_l, int pos_c)
{
    if (((line >= (pos_l - 1)) && (line <= (pos_l + 1))) && ((column >= (pos_c - 1)) && (column <= (pos_c + 1))))
    {
        return true;
    }
    return false;
}

static void *thread_func(void *arg)
{
    Block units[10][10] = {
        {0, 1, 1, 1, 1, 0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 1, 0, 1, 1},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 0, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        {1, 0, 0, 0, 1, 0, 0, 1, 1, 1}};

    int pos_c = 4;
    int pos_l = 1;

    while (true)
    {
        clear_screen();
        reset_cursor();

        OPER op = OPER_INVALID;
        bool keyin = operQueue.try_pop(op);
        // cout << "receive:" << op << endl;
        if (keyin)
        {
            switch (op)
            {
            case OPER_DOWN:
                pos_l = ((pos_l + 1) < 10 && units[pos_l + 1][pos_c].isBlank()) ? pos_l + 1 : pos_l;
                break;
            case OPER_UP:
                pos_l = ((pos_l - 1) >= 0 && units[pos_l - 1][pos_c].isBlank()) ? pos_l - 1 : pos_l;
                break;
            case OPER_LEFT:
                pos_c = ((pos_c - 1) >= 0 && units[pos_l][pos_c - 1].isBlank()) ? pos_c - 1 : pos_c;
                break;
            case OPER_RIGHT:
                pos_c = ((pos_c + 1) < 10 && units[pos_l][pos_c + 1].isBlank()) ? pos_c + 1 : pos_c;
                break;
            }
        }

        for (size_t line = 0; line < 10; line++)
        {
            for (size_t column = 0; column < 10; column++)
            {
                if (line == pos_l && column == pos_c)
                {
                    cout << "👧";
                }
                else if (need_print(line, column, pos_l, pos_c))
                {
                    cout << units[line][column].str;
                }
                else
                {
                    cout << "**";
                }
            }
            cout << endl;
        }
        cout << endl;
        usleep(20000);
    }
}

static OPER get_user_op(int keyin)
{
    OPER op;
    switch (keyin)
    {
    case 'w':
        op = OPER_UP;
        break;
    case 'a':
        op = OPER_LEFT;
        break;
    case 's':
        op = OPER_DOWN;
        break;
    case 'd':
        op = OPER_RIGHT;
        break;
    default:
        op = OPER_INVALID;
        break;
    }
    return op;
}

char nonblocking_input(void)
{
    char input_ch;
    struct termios newt, oldt;

    int tty = open("/dev/tty", O_RDONLY); // Open control terminal

    tcgetattr(tty, &oldt); // Get terminal properties
    newt = oldt;

    // Set characters are not buffered(~ICANON) and do not echo(~ECHO).
    // You can also choose only one of them.
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(tty, TCSANOW, &newt);

    read(tty, &input_ch, 1);
    tcsetattr(tty, TCSANOW, &oldt); // Restore terminal properties

    return input_ch;
}

int main()
{
    srand(time(NULL)); // use current time as seed for random generator
    init_pics();

    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    OPER op = OPER_INVALID;
    int keyin = '\0';
    while (true)
    {
        keyin = nonblocking_input();
        op = get_user_op(keyin);

        if (op != OPER_INVALID)
        {
            operQueue.push(op);
        }
        usleep(20000);
    }

    pthread_join(tid, NULL);
    return 0;
}