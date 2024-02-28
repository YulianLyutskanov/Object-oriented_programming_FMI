#include <iostream>
using std::cin;
using std::cout;
using std::endl;

const short bufferSize = 15;

void lowerCase(char *str)
{

    while (*str)
    {
        *str = tolower(*str);
        str++;
    }
}

namespace Vectors
{
struct TriVector
{
    int x = 0, y = 0, z = 0;
};

TriVector sum(const TriVector &first, const TriVector &second)
{
    return {first.x + second.x, first.y + second.y, first.z + second.z};
}

void printVec(const TriVector &vec)
{
    cout << "x: " << vec.x << ", y: " << vec.y << ", z: " << vec.z;
}

bool areLinearDependent(const TriVector &first, const TriVector &second)
{
    return ((first.x * second.y - first.y * second.x == 0) && (first.x * second.z - first.z * second.x == 0) &&
            (first.y * second.z - first.z * second.y == 0)); // determinant form
}

} // namespace Vectors

namespace Tutors
{

enum class EducationalLevel
{
    BACHELOR,
    MASTER,
    PHD,
    UNKNOWN
};

struct Tutor
{
    unsigned id = 0;
    EducationalLevel degree = EducationalLevel::UNKNOWN;
    unsigned countFailedSTudents = 0;
};

const char *getDegreeStr(EducationalLevel l)
{
    switch (l)
    {
    case Tutors::EducationalLevel::BACHELOR:
        return "Bachelor";
    case Tutors::EducationalLevel::MASTER:
        return "Master";
    case Tutors::EducationalLevel::PHD:
        return "PhD";
    default:
        return "Unknown";
    }
}

EducationalLevel getDegreeEnum(char *degree)
{
    lowerCase(degree);
    if (strcmp(degree, "bachelor") == 0)
        return EducationalLevel::BACHELOR;
    else if (strcmp(degree, "master") == 0)
        return EducationalLevel::MASTER;
    else if (strcmp(degree, "phd") == 0)
        return EducationalLevel::PHD;
    else
        return EducationalLevel::UNKNOWN;
}
void initTutor(Tutor &t, unsigned id, EducationalLevel e, unsigned failedNum)
{
    if (e == EducationalLevel::UNKNOWN)
    {
        cout << "Invalid level of education";
        return;
    }

    t.id = id;
    t.degree = e;
    t.countFailedSTudents = failedNum;
}

void printTutor(const Tutor &t)
{
    cout << "ID: " << t.id << ", Degree: " << getDegreeStr(t.degree)
         << ", Number of failed students: " << t.countFailedSTudents;
}
void readTutor(Tutor &t)
{

    unsigned id, failedCount;
    char education[bufferSize];
    cin >> id >> education >> failedCount;
    t.id = id;
    t.countFailedSTudents = failedCount;
    t.degree = getDegreeEnum(education);
}
} // namespace Tutors

namespace University
{
using namespace Tutors;

constexpr short TUTORS_COUNT = 5;
struct Uni
{
    int tutorsCount = TUTORS_COUNT;
    Tutor TutorsList[TUTORS_COUNT];
    double avgFailed = 0;
};

double calcAvgFailed(const Uni &uni)
{
    int sumOfFailed = 0;
    for (size_t i = 0; i < uni.tutorsCount; i++)
    {
        sumOfFailed += uni.TutorsList[i].countFailedSTudents;
    }
    return (double)sumOfFailed / uni.tutorsCount;
}

void readUni(Uni &uni)
{
    for (size_t i = 0; i < TUTORS_COUNT; i++)
    {
        Tutor t;
        readTutor(t);
        uni.TutorsList[i] = t;
    }
    uni.avgFailed = calcAvgFailed(uni);
}

void printUni(const Uni &uni)
{
    for (size_t i = 0; i < uni.tutorsCount; i++)
    {
        printTutor(uni.TutorsList[i]);
        cout << endl;
    }
    cout << "avg failed: " << uni.avgFailed;
}

unsigned calcBonuses(const Uni &uni, unsigned minFailed)
{
    unsigned res = 0;
    for (size_t i = 0; i < uni.tutorsCount; i++)
    {
        if (uni.TutorsList[i].countFailedSTudents >= minFailed)
            res++;
    }
    return res;
}

void sortByFailedStudents(Uni &uni)
{
    for (size_t i = 0; i < uni.tutorsCount; i++)
    {
        unsigned minIndex = i;
        for (size_t j = i + 1; j < uni.tutorsCount; j++)
        {
            if (uni.TutorsList[j].countFailedSTudents < uni.TutorsList[minIndex].countFailedSTudents)
                minIndex = j;
        }
        if (minIndex != i)
            std::swap(uni.TutorsList[minIndex], uni.TutorsList[i]);
    }
}

} // namespace University

namespace Graphs
{
const unsigned MAX_NAME_LENGTH = 10;

struct Vertex
{
    char name[MAX_NAME_LENGTH] = {};
};

struct Edge
{
    Vertex v1;
    Vertex v2;
};

struct Graph
{
    unsigned verticesCount = 0;
    unsigned edgesCount = 0;
    Vertex *verticesList = nullptr;
    Edge *edgesList = nullptr;
};

void initGraph(Graph &g)
{
    int m;
    cin >> m;
    g.edgesList = new Edge[m];
    g.edgesCount = m;
}

void deleteGraph(Graph &g)
{
    delete[] g.verticesList;
    delete[] g.edgesList;
    g.verticesList = nullptr;
    g.edgesList = nullptr;
    g.edgesCount = 0;
    g.verticesCount = 0;
}

bool isVertexInGraph(const Graph &g, const Vertex &v)
{
    if (g.verticesCount == 0)
        return false;

    for (size_t i = 0; i < g.verticesCount; i++)
    {
        if (strcmp(g.verticesList[i].name, v.name) == 0)
            return true;
    }
    return false;
}

void addVertex(Graph &g, const Vertex &v)
{
    if (!isVertexInGraph(g, v))
    {
        Vertex *newVerticesList = new Vertex[g.verticesCount + 1];

        for (size_t i = 0; i < g.verticesCount; i++) // copy old list
        {
            newVerticesList[i] = g.verticesList[i];
        }
        newVerticesList[g.verticesCount] = v; // add last new element
        delete[] g.verticesList;
        g.verticesList = newVerticesList;
        g.verticesCount++;
    }
}

void addEdge(Graph &g, const Vertex &v1, const Vertex &v2)
{
    addVertex(g, v1);
    addVertex(g, v2);

    Edge _new = {v1, v2};

    Edge *newEdgesList = new Edge[g.edgesCount + 1];

    for (size_t i = 0; i < g.edgesCount; i++) // copy the old graph
    {
        newEdgesList[i] = g.edgesList[i];
    }

    newEdgesList[g.edgesCount] = _new; // add last new element
    delete[] g.edgesList;
    g.edgesList = newEdgesList;
    g.edgesCount++;
}

unsigned powOfVertexOut(const Graph &g, const Vertex &v)
{
    unsigned result = 0;
    for (size_t i = 0; i < g.edgesCount; i++)
    {
        if (strcmp(v.name, g.edgesList[i].v2.name) == 0)
        {
            result++;
        }
    }
    return result;
}

unsigned powOfVertexIn(const Graph &g, const Vertex &v)
{
    unsigned result = 0;
    for (size_t i = 0; i < g.edgesCount; i++)
    {
        if (strcmp(v.name, g.edgesList[i].v1.name) == 0)
        {
            result++;
        }
    }
    return result;
}

Graph inputGraph()
{
    int m;
    cin >> m;
    Graph res;

    for (size_t i = 0; i < m; i++)
    {
        Vertex v1;
        Vertex v2;
        cin >> v1.name >> v2.name;
        addEdge(res, v1, v2);
    }
    return res;
}

bool isGraphFull(const Graph &g)
{
    for (size_t i = 0; i < g.verticesCount; i++)
    {
        if (powOfVertexOut(g, g.verticesList[i]) != g.verticesCount - 1)
            return false;
    }

    return true;
}

bool areEdgesEqual(const Edge &e1, const Edge &e2)
{
    return strcmp(e1.v1.name, e2.v1.name) == 0 && strcmp(e1.v2.name, e2.v2.name) == 0;
}

void deleteEdge(Graph &g, const Edge &e)
{
    for (size_t i = 0; i < g.edgesCount; i++)
    {
        if (areEdgesEqual(e, g.edgesList[i]))
        {
            Edge *newEdgesList = new Edge[g.edgesCount - 1];

            for (size_t j = 0; j < i; j++) // copy the old list without the deleted edge
            {
                newEdgesList[j] = g.edgesList[j];
            }
            for (size_t j = i; j < g.edgesCount - 1; j++)
            {
                newEdgesList[j] = g.edgesList[j + 1];
            }
            delete[] g.edgesList;
            g.edgesList = newEdgesList;
            g.edgesCount--;
            return;
        }
    }
    cout << "There isn't such an edge in this graph";
}
} // namespace Graphs

int main()
{
    // using namespace Vectors;

    // TriVector vec1 = {1, 2, 3};
    // TriVector vec2 = {2, 4, 7};

    // printVec(sum(vec1, vec2));
    // cout << areLinearDependent(vec1, vec2);

    // using namespace Tutors;
    // Tutor az;
    // initTutor(az, 3435, EducationalLevel::BACHELOR, 69);
    // printTutor(az);

    // using namespace University;
    // Uni uni;
    // readUni(uni);
    // printUni(uni);
    // cout << endl;
    // unsigned failedCount = 30;
    // cout << calcBonuses(uni, failedCount);
    // sortByFailedStudents(uni);
    // printUni(uni);

    using namespace Graphs;
    Graph test = inputGraph();
    // Vertex v = {"Eli"};
    // cout << powOfVertex(test, v);
    // cout << isGraphFull(test);
    deleteEdge(test, {"Vili", "Eli"});
}

/*
1111 phd 68
2222 gri 44
333 master 445
4444 bachelor 0
55555 sdfdfs 3
*/

/*
5
Vili Eli
Eli Lubo
Lubo Eli
Plami Ivan
Plami Desi
*/
