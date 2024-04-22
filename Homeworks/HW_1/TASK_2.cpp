#include <fstream>
#include <iomanip>
#include <iostream>

#pragma warning(disable : 4996)
using std::cout;

namespace GlobalConstants
{
const uint16_t MAX_SONGNAME_LENGTH = 64;
const uint16_t MAX_SONG_CONTENT_LENGTH_BYTES = 256;
const uint16_t MAX_SONGS_IN_PLAYLIST = 30;
const uint16_t MINUTES_IN_HOUR = 60;
const uint16_t SECONDS_IN_MINUTE = 60;
const uint16_t BUFFER_SIZE = 100;
const uint16_t BITS_IN_BYTE = 8;
} // namespace GlobalConstants
namespace HelpingFunctions
{
char myToLower(char ch)
{
    if (!(ch >= 'A' && ch <= 'Z'))
        return ch;

    return ch - ('A' - 'a');
}

int myMin(int a, int b)
{
    return a < b ? a : b;
}

int32_t bytesInFile(std::ifstream &ifs)
{
    if (!ifs.is_open())
        return -1;

    int32_t curPos = ifs.tellg();
    ifs.seekg(0, std::ios::end);

    int32_t endIndex = ifs.tellg();

    ifs.seekg(curPos);

    return endIndex;
}

int32_t bytesInFile(const char *fileName)
{
    if (!fileName)
        return -2; // error code

    std::ifstream ifs(fileName, std::ios::binary);
    if (!ifs.is_open())
        return -1; // error code

    int res = bytesInFile(ifs);
    ifs.close();

    return res;
}

void setBit(char &ch, uint16_t position) // indexing from 0...7
{
    if (position > 7)
        return;
    ch |= (1 << (GlobalConstants::BITS_IN_BYTE - position - 1)); // starting from left to right
}

void printBits(char ch)
{
    for (int16_t i = (GlobalConstants::BITS_IN_BYTE - 1); i >= 0; i--)
    {
        cout << (bool)(ch & (1 << i));
    }
}

void copyCharArr(char *dest, const char *source, size_t length)
{
    if (!source || !dest)
        return;
    for (size_t i = 0; i < length; i++)
    {
        dest[i] = source[i];
    }
}

} // namespace HelpingFunctions
namespace GenreInterface
{
enum class Genre : unsigned char
{
    ROCK = 1 << 0,
    POP = 1 << 1,
    HIP_HOP = 1 << 2,
    ELECTRO = 1 << 3,
    JAZZ = 1 << 4,
    UNKNOWN = 0
};
const char *getStringRepresentation(Genre genre)
{
    char buff[GlobalConstants::BUFFER_SIZE] = {};
    if (!(int)genre)
    {
        strcat(buff, "&Unknown");
    }
    else
    {
        if ((int)genre & (int)Genre::ROCK)
            strcat(buff, "&Rock");
        if ((int)genre & (int)Genre::POP)
            strcat(buff, "&Pop");
        if ((int)genre & (int)Genre::HIP_HOP)
            strcat(buff, "&Hip-Hop");
        if ((int)genre & (int)Genre::ELECTRO)
            strcat(buff, "&Electronic");
        if ((int)genre & (int)Genre::JAZZ)
            strcat(buff, "&Jazz");
    }

    char *res = new char[strlen(buff)]; // without the first & symbol

    strcpy(res, &buff[1]);

    return res;
}
Genre getGenreFromChar(char ch)
{

    switch (HelpingFunctions::myToLower(ch))
    {
    case 'r':
        return Genre::ROCK;
    case 'p':
        return Genre::POP;
    case 'h':
        return Genre::HIP_HOP;
    case 'e':
        return Genre::ELECTRO;
    case 'j':
        return Genre::JAZZ;
    default:
        return Genre::UNKNOWN;
    }
}
} // namespace GenreInterface

using namespace GlobalConstants;
using namespace GenreInterface;
using namespace HelpingFunctions;

typedef char byte;

class Time
{
  private:
    uint32_t seconds = 0;

    uint32_t getTotalMinutes() const
    {
        return seconds / SECONDS_IN_MINUTE;
    }
    uint32_t getTotalSeconds() const
    {
        return seconds;
    }

  public:
    Time()
    {
        seconds = 0;
    }
    Time(uint32_t hours, uint16_t minutes, uint16_t seconds)
    {
        this->setHours(hours);
        this->setMinutes(minutes);
        this->setSeconds(seconds);
    }

    void setSeconds(uint16_t _seconds)
    {
        seconds = seconds - this->getSeconds() + _seconds;
    }
    void setMinutes(uint16_t minutes)
    {
        seconds = seconds - this->getMinutes() * SECONDS_IN_MINUTE + minutes * SECONDS_IN_MINUTE;
    }
    void setHours(uint32_t hours)
    {
        seconds = seconds - this->getHours() * MINUTES_IN_HOUR * SECONDS_IN_MINUTE +
                  hours * MINUTES_IN_HOUR * SECONDS_IN_MINUTE;
    }

    uint16_t getMinutes() const
    {
        return this->getTotalMinutes() % MINUTES_IN_HOUR;
    }
    uint16_t getSeconds() const
    {
        return seconds % SECONDS_IN_MINUTE;
    }
    uint32_t getHours() const
    {
        return this->getTotalMinutes() / MINUTES_IN_HOUR;
    }

    void print() const
    {
        uint32_t hours_ = this->getHours();
        uint16_t minutes_ = this->getMinutes(), seconds_ = this->getSeconds();

        // printing with leading zeros if needed
        cout << std::setw(2) << std::setfill('0') << hours_ << ':' << std::setw(2) << std::setfill('0') << minutes_
             << ':' << std::setw(2) << std::setfill('0') << getSeconds();
    }
};

class Melody
{
  private:
    byte content[MAX_SONG_CONTENT_LENGTH_BYTES] = {};
    uint16_t length = 0; // in bytes
    bool isGood = true;

  public:
    Melody()
    {
        length = 0;
        isGood = this->setContent("", 0);
    }
    Melody(const byte *_content, uint16_t _length)
    {
        isGood = this->setContent(_content, _length);
    }

    const byte *getContent() const
    {
        return content;
    }
    uint16_t getLength() const
    {
        return length;
    }
    bool isOK() const
    {
        return isGood;
    }

    bool setContent(const byte *_content, uint16_t _length)
    {
        if (_length > MAX_SONG_CONTENT_LENGTH_BYTES || !content)
            return false;

        copyCharArr(content, _content, _length);
        length = _length;
        return true;
    }

    void changeRhythm(uint16_t tactInterval)
    {
        if (tactInterval == 0) // we want a positive step
            return;

        uint16_t curBit = this->getLength() * BITS_IN_BYTE - tactInterval; // starting from the end , counting is from 0
        while (curBit < MAX_SONG_CONTENT_LENGTH_BYTES * BITS_IN_BYTE)
        {
            setBit(content[curBit / BITS_IN_BYTE], curBit % BITS_IN_BYTE);
            curBit -= tactInterval;
        }
    }
    void print() const
    {
        for (uint16_t i = 0; i < length; i++)
        {
            printBits(content[i]);
        }
    }
};

class Song
{
  private:
    char name[MAX_SONGNAME_LENGTH] = {};
    Time duration;
    Genre genre = Genre::UNKNOWN;
    Melody melody;
    bool isGood = true;

  public:
    Song()
    {
    }
    Song(const char *_name, const Time &_duration, Genre _genre, const Melody _melody)
    {
        if (!name)
        {
            isGood = false;
            return;
        }

        strcpy(name, _name);
        duration = _duration;
        genre = _genre;
        melody = _melody;
    }

    const char *getName() const
    {
        return name;
    }
    const Time &getDuration() const
    {
        return duration;
    }
    const Genre &getGenre() const
    {
        return genre;
    }
    const Melody &getMelody() const
    {
        return melody;
    }
    bool isOK() const
    {
        return isGood;
    }

    void setName(const char *str)
    {
        if (!str)
        {
            isGood = false;
            return;
        }
        strcpy(name, str);
    }
    void setDuration(uint32_t hours, uint16_t minutes, uint16_t seconds)
    {
        duration.setHours(hours);
        duration.setMinutes(minutes);
        duration.setSeconds(seconds);
    }
    void setGenre(Genre _genre)
    {
        genre = _genre;
    }
    void setMelody(Melody _melody)
    {
        melody = _melody;
    }

    void changeRhythm(uint16_t tactInterval)
    {
        melody.changeRhythm(tactInterval);
    }
    void save(const char *destName) const
    {
        if (!destName)
            return;

        std::ofstream ofs(destName, std::ios::binary);
        if (!ofs.is_open())
            return;
        ofs.write(this->getMelody().getContent(), sizeof(byte) * this->getMelody().getLength());
        ofs.close();
    }

    void print() const
    {
        cout << name << ", ";
        duration.print();
        const char *temp_str = getStringRepresentation(genre);
        cout << ", " << temp_str;
        delete[] temp_str;
    }
};

class Playlist
{
  private:
    uint16_t songsCount = 0;
    Song songs[MAX_SONGS_IN_PLAYLIST];
    // uint16_t songsCount[]; // we could fill the padding to save memory

  public:
    uint16_t getSongsCount() const
    {
        return songsCount;
    }
    const Song *getSongs() const
    {
        return songs;
    }

    bool add(const char *songName, unsigned hours, unsigned minutes, unsigned seconds, const char *genre,
             const char *sourceFile)
    {
        if (songsCount >= MAX_SONGS_IN_PLAYLIST)
            return false;

        if (!songName || !sourceFile || !genre)
            return false;

        std::ifstream ifs(sourceFile, std::ios::binary);

        if (!ifs.is_open())
            return false;

        songs[songsCount].setName(songName);                    // init name
        songs[songsCount].setDuration(hours, minutes, seconds); // init duration

        byte buff[MAX_SONG_CONTENT_LENGTH_BYTES];
        ifs.read(buff, sizeof(buff)); // read content
        ifs.clear();
        uint16_t newMelodyLength = bytesInFile(ifs);
        ifs.close();
        if (newMelodyLength >= MAX_SONG_CONTENT_LENGTH_BYTES || newMelodyLength == -1)
            return false;
        Melody newMelody(buff, newMelodyLength);
        songs[songsCount].setMelody(newMelody); // init melody

        Genre newGenre = Genre::UNKNOWN;
        while (*genre) // init genre
            newGenre = (Genre)((unsigned char)newGenre |
                               (unsigned char)getGenreFromChar(*(genre++))); // get all Genre configuration

        songs[songsCount].setGenre(newGenre);

        songsCount++;
        return true;
    }
    void print() const
    {
        for (uint16_t i = 0; i < songsCount; i++)
        {
            songs[i].print();
            cout << std::endl;
        }
    }
    int16_t find(const char *name) const
    {
        if (!name)
            return -2;
        for (uint16_t i = 0; i < songsCount; i++)
        {
            if (strcmp(name, songs[i].getName()) == 0)
            {
                // songs[i].print();
                return i;
            }
        }
        return -1; // not found
    }
    void findGenre(char ch) const
    {
        for (uint16_t i = 0; i < songsCount; i++)
        {
            if ((unsigned char)songs[i].getGenre() & (unsigned char)getGenreFromChar(ch)) // if they have common Genres
            {
                songs[i].print();
                cout << std::endl;
            }
        }
    }
    void selectionSortByComparator(bool (*isLess)(const Song &, const Song &))
    {

        for (uint16_t i = 0; i < songsCount - 1; i++)
        {
            uint16_t minElIndex = i;
            for (uint16_t j = i + 1; j < songsCount; j++)
            {
                if (isLess(songs[j], songs[minElIndex]))
                {
                    minElIndex = j;
                }
            }
            if (minElIndex != i)
                std::swap(songs[i], songs[minElIndex]);
        }
    }
    void sortByName()
    {
        selectionSortByComparator(
            [](const Song &first, const Song &second) { return strcmp(first.getName(), second.getName()) < 0; });
    }
    void sortByDuration()
    {
        selectionSortByComparator([](const Song &first, const Song &second) {
            return first.getDuration().getSeconds() < second.getDuration().getSeconds();
        });
    }
    void save(const char *songName, const char *destName) const
    {
        if (!songName || !destName)
            return;

        int16_t songIndex = this->find(songName);
        if (songIndex == -1)
            return; // not found

        std::ofstream ofs(destName, std::ios::binary);
        if (!ofs.is_open())
            return;

        ofs.write(songs[songIndex].getMelody().getContent(), sizeof(byte) * songs[songIndex].getMelody().getLength());
        ofs.close();
    }
    void mix(const char *song1Name, const char *song2Name)
    {
        if (!song1Name || !song2Name)
            return;

        uint16_t song1Index = this->find(song1Name);
        uint16_t song2Index = this->find(song2Name);
        if (song1Index == -1 || song2Index == -1)
            return; // song not found
        uint16_t minLengthOfMelody =
            myMin(songs[song1Index].getMelody().getLength(), songs[song2Index].getMelody().getLength());

        byte newContent[BUFFER_SIZE];
        for (uint16_t i = 0; i < minLengthOfMelody; i++)
        {
            newContent[i] =
                songs[song1Index].getMelody().getContent()[i] ^ songs[song2Index].getMelody().getContent()[i];
        }
        Melody newMelody(newContent, songs[song1Index].getMelody().getLength());
        songs[song1Index].setMelody(newMelody);
    }
};

int main()
{
    Playlist p;
    p.add("Song 2", 0, 1, 55, "rp", "song2.txt");
    p.add("Song 1", 0, 1, 5, "p", "song1.txt");
    p.add("A1", 0, 1, 5, "p", "song1.txt");

    p.print();
    //  p.getSongs()[p.find("Song 1")].print();
    // p.findGenre('p');
    // p.findGenre('r');
    // p.sortByName();
    // p.sortByDuration();
    // p.print();
    // p.save("Song 2", "test.txt");
    // p.mix("Song 1", "Song 2");

    // p.save("Song 1", "test2.txt");
    // Song my = p.getSongs()[p.find("Song 2")];
    // Melody m("\0\0", 2);
    // my.setMelody(m);
    // my.changeRhythm(5);
    // my.getMelody().print();
}
