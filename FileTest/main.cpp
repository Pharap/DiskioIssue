#include <Pokitto.h>
#include <SDFileSystem.h>
#include <cstdint>

SDFileSystem sd
(
	/*MOSI*/P0_9,
	/*MISO*/P0_8,
	/*SCK*/P0_6,
	/*CS*/P0_7,
	/*Mountpoint*/"sd",
	NC,
	SDFileSystem::SWITCH_NONE,
	25000000
);

// This is a data structure containing the highscore data
struct HighScoreEntry
{
	char name[3];
	unsigned int highScore;
};

// This is an array that holds your highscore entries
constexpr const std::size_t HighscoreCount = 5;
HighScoreEntry highScoreTable[HighscoreCount];

// It's better to have this as a global constant so it's easy to change
char highScoreFileName[] = "/sd/scores.txt";

int saveAllHighScores(void)
{
	FILE * file = fopen(highScoreFileName, "w");

	if(file == nullptr)
		return -1;

	fseek(file, 0, SEEK_SET);

	if(ferror(file) != 0)
	{
		fclose(file);
		return -2;
	}

	const std::size_t amount = fwrite(highScoreTable, sizeof(highScoreTable[0]), HighscoreCount, file);

	if(ferror(file) != 0)
	{
		fclose(file);
		return -3;
	}

	fclose(file);

	return (amount >= HighscoreCount) ? 0 : -4;
}

int loadAllHighScores(void)
{
	FILE * file = fopen(highScoreFileName, "w");

	if(file == nullptr)
		return -1;

	fseek(file, 0, SEEK_SET);

	if(ferror(file) != 0)
	{
		fclose(file);
		return -2;
	}

	const std::size_t amount = fread(highScoreTable, sizeof(highScoreTable[0]), HighscoreCount, file);

	if(ferror(file) != 0)
	{
		fclose(file);
		return -3;
	}

	fclose(file);

	return (amount >= HighscoreCount) ? 0 : -4;
}

Pokitto::Core mygame;
int selected = 0;

void printScore(const HighScoreEntry & entry)
{
	for (int i = 0; i < 3; i++)
	{
		Pokitto::Display::print(entry.name[i]);
		Pokitto::Display::print(' ');
	}
	Pokitto::Display::println(entry.highScore);
}

int saveSuccess = 0;
int loadSuccess = 0;

int main (void)
{
    Pokitto::Core::begin();
	std::memset(highScoreTable, 0, sizeof(highScoreTable));
    for (int i = 0; i < 5; ++i)
	{
		for (int k = 0; k < 3; ++k)
			highScoreTable[i].name[k] = ('A' + i);

        highScoreTable[i].highScore = 5 - i;
    }

    while (Pokitto::Core::isRunning())
	{
        if (Pokitto::Core::update())
		{
            Pokitto::Display::setColor(1);
            Pokitto::Display::println("Hello World!");
            Pokitto::Display::print(sizeof(highScoreTable));
            Pokitto::Display::print(' ');
            Pokitto::Display::print(sizeof(HighScoreEntry));
            Pokitto::Display::print(' ');
            Pokitto::Display::print(sizeof(highScoreTable) / sizeof(HighScoreEntry));
            Pokitto::Display::println();
            Pokitto::Display::println(loadSuccess);
            Pokitto::Display::println(saveSuccess);

            if (Pokitto::Core::buttons.pressed(BTN_A))
			{
				saveSuccess = saveAllHighScores();
            }

            if (Pokitto::Core::buttons.pressed(BTN_B))
			{
				loadSuccess = loadAllHighScores();
            }

            Pokitto::Display::setColor(1);

			for (int i = 0; i < 5; ++i)
                printScore(highScoreTable[i]);
        }
    }
    return 0;
}
