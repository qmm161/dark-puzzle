#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

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
    cout << "idx:" << idx << " " << c << endl;
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

    const char *str;

private:
    static char BLANK[3];
} Block;

char _Block::BLANK[3] = {' ', ' ', '\0'};

int main()
{
    srand(time(NULL)); // use current time as seed for random generator
    init_pics();

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

    for (int line = 0; line < 10; line++)
    {
        for (auto u : units[line])
        {
            cout << u.str;
        }
        cout << endl;
    }
    cout << endl;

    return 0;
}