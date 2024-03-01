#include <iostream>
#include <queue>
#include <algorithm>
#include <functional>

using namespace std;

static int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
static int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

class Tree
{
public:
    int x, y, age;

    Tree(int x, int y, int age) : x(x), y(y), age(age) {}

    bool operator<(const Tree &t) const
    {
        return this->age > t.age;
    }
};

static int A[10][100];
static int nutrient[10][100];
static int N, M, K;
static queue<Tree> born;
static priority_queue<Tree> pq;
static queue<Tree> dead;

void spring()
{
    /*
    봄에는 나무가 자신의 나이만큼 양분을 먹고, 나이가 1 증가한다.
    각각의 나무는 나무가 있는 1×1 크기의 칸에 있는 양분만 먹을 수 있다.
    하나의 칸에 여러 개의 나무가 있다면, 나이가 어린 나무부터 양분을 먹는다.
    만약, 땅에 양분이 부족해 자신의 나이만큼 양분을 먹을 수 없는 나무는 양분을 먹지 못하고 즉시 죽는다.
    */
    priority_queue<Tree> temp;
    while (!pq.empty())
    {
        Tree t = pq.top();
        pq.pop();
        if (nutrient[t.x - 1][t.y - 1] >= t.age)
        {
            if ((t.age + 1) % 5 == 0)
            {
                born.push(Tree(t.x, t.y, t.age + 1));
            }
            temp.push(Tree(t.x, t.y, t.age + 1));
            nutrient[t.x - 1][t.y - 1] -= t.age;
        }
        else
        {
            dead.push(t);
        }
    }
    pq = temp;
}

void summer()
{
    /*
    여름에는 봄에 죽은 나무가 양분으로 변하게 된다.
    각각의 죽은 나무마다 나이를 2로 나눈 값이 나무가 있던 칸에 양분으로 추가된다.
    소수점 아래는 버린다.
    */
    while (!dead.empty())
    {
        Tree t = dead.front();
        dead.pop();
        nutrient[t.x - 1][t.y - 1] += t.age / 2;
    }
}

void fall()
{
    /*
    가을에는 나무가 번식한다.
    번식하는 나무는 나이가 5의 배수이어야 하며, 인접한 8개의 칸에 나이가 1인 나무가 생긴다.
    어떤 칸 (r, c)와 인접한 칸은 (r-1, c-1), (r-1, c), (r-1, c+1), (r, c-1), (r, c+1), (r+1, c-1), (r+1, c), (r+1, c+1) 이다.
    상도의 땅을 벗어나는 칸에는 나무가 생기지 않는다.
    */
    while (!born.empty())
    {
        Tree t = born.front();
        born.pop();
        for (int i = 0; i < 8; i++)
        {
            int nx = t.x + dx[i];
            int ny = t.y + dy[i];
            if (nx < 1 || nx > N || ny < 1 || ny > N)
                continue;
            pq.push(Tree(nx, ny, 1));
        }
    }
}

void winter()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            nutrient[i][j] += A[i][j];
        }
    }
}

int main()
{
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> A[i][j];
            nutrient[i][j] = 5;
        }
    }

    for (int i = 0; i < M; i++)
    {
        int x, y, z;
        cin >> x >> y >> z;
        pq.push(Tree(x, y, z));
    }

    int year = 0;
    while (year < K)
    {
        spring();
        summer();
        fall();
        winter();
        year++;
    }

    cout << pq.size() << endl;
    return 0;
}