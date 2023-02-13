#pragma once
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>             // Required for: getch(), chdir() (POSIX), access()
#include <dirent.h>  
#include <sys/stat.h>               // Required for: stat() [Used in GetFileModTime()]
#define GETCWD getcwd
#define CHDIR chdir

#define MAX_DIRECTORY_FILES     512
#define MAX_FILEPATH_LENGTH      512  
#define MAX_TEXT_BUFFER_LENGTH              1024        // Size of internal static buffers used on some functions:
                                                        // TextFormat(), TextSubtext(), TextToUpper(), TextToLower(), TextToPascal(), TextSplit()
#define MAX_TEXT_UNICODE_CHARS               512        // Maximum number of unicode codepoints: GetCodepoints()
#define MAX_TEXTSPLIT_COUNT                  128        // Maximum number of substrings to split: TextSplit()


#define GLSL(src) "#version 320 es\n" #src

#define DWORD					long
#define WORD					short
#define INT						int
#define ULONG					unsigned long
#define UINT					unsigned int
#define BOOL					int
#define LONG					long
#define	BYTE					unsigned char
#define FLOAT                   float


#define CONSOLE_COLOR_RESET "\033[0m"
#define CONSOLE_COLOR_GREEN "\033[1;32m"
#define CONSOLE_COLOR_RED "\033[1;31m"
#define CONSOLE_COLOR_PURPLE "\033[1;35m"
#define CONSOLE_COLOR_CYAN "\033[0;36m"





inline	void Log(int severity, const char* fmt, ...)
{

		const char* type;
		const char* color;
		switch (severity) 
        {
			case  0:
				type = "info";
				color = CONSOLE_COLOR_GREEN;
				break;
			case 1:
				type = "warning";
				color = CONSOLE_COLOR_PURPLE;
				break;
            case 2:
				type = "error";
				color = CONSOLE_COLOR_RED;
				break;
			default:
				break;
		}


		time_t rawTime;
		struct tm* timeInfo;
		char timeBuffer[80];

		time(&rawTime);
		timeInfo = localtime(&rawTime);

		strftime(timeBuffer, sizeof(timeBuffer), "[%H:%M:%S]", timeInfo);

		char consoleFormat[1024];
		snprintf(consoleFormat, 1024, "%s%s %s%s%s: %s\n", CONSOLE_COLOR_CYAN,
				timeBuffer, color, type, CONSOLE_COLOR_RESET, fmt);


		char fileFormat[1024];
		snprintf(fileFormat, 1024, "%s %s: %s\n", timeBuffer, type, fmt);

		va_list argptr;

		va_start(argptr, fmt);
		vprintf(consoleFormat, argptr);
		va_end(argptr);

		char buf[4096];

		va_start(argptr, fmt);
		vsprintf(buf, fmt, argptr);
		va_end(argptr);
}

inline std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters)
{
		size_t start = 0;
		size_t end = string.find_first_of(delimiters);

		std::vector<std::string> result;

		while (end <= std::string::npos)
            {
			std::string token = string.substr(start, end - start);
			if (!token.empty())
			{
				result.push_back(token);
			}

			if (end == std::string::npos)
                {
				break;
			}

			start = end + 1;
			end = string.find_first_of(delimiters, start);
		}

		return result;

}
inline bool find_word(const std::string& haystack,const std::string& needle)
{
    int index = (int)haystack.find(needle);
    if (index == (int)std::string::npos) return false;

    auto not_part_of_word = [&](int index){
        if (index < 0 || index >= (int)haystack.size()) return true;
        if (std::isspace(haystack[index]) || std::ispunct(haystack[index])) return true;
        return false;
    };
    return not_part_of_word(index-1) && not_part_of_word(index+needle.size());
}



inline unsigned char *LoadFileData(const char *fileName, unsigned int *bytesRead)
{
   unsigned char *data = NULL;
    *bytesRead = 0;

    if (fileName != NULL)
    {
        SDL_RWops* file= SDL_RWFromFile(fileName, "rb");


        if (file != NULL)
        {
           unsigned int size =(int) SDL_RWsize(file);

            if (size > 0)
            {
                data = (unsigned char *)malloc(size*sizeof(unsigned char));

                unsigned int count = (unsigned int) SDL_RWread(file, data, sizeof(unsigned char), size);
                *bytesRead = count;

                if (count != size) Log(1, "FILEIO: [%s] File partially loaded", fileName);
                else  Log(0, "FILEIO: [%s] File loaded successfully", fileName);
            }
            else Log(1, "FILEIO: [%s] Failed to read file", fileName);
            SDL_RWclose(file);
        }
        else Log(1, "FILEIO: [%s] Failed to open file", fileName);
    }
    else Log(1, "FILEIO: File name provided is not valid");
    return data;
}

inline bool SaveFileData(const char *fileName, void *data, unsigned int bytesToWrite)
{
    bool success = false;

    if (fileName != NULL)
    {
        SDL_RWops* file= SDL_RWFromFile(fileName, "wb");
        if (file != NULL)
        {
             unsigned int count = (unsigned int) SDL_RWwrite(file, data, sizeof(unsigned char), bytesToWrite);
             if (count == 0) Log(2, "FILEIO: [%s] Failed to write file", fileName);
            else if (count != bytesToWrite) Log(1, "FILEIO: [%s] File partially written", fileName);
            else  Log(0, "FILEIO: [%s] File saved successfully", fileName);

            int result = SDL_RWclose(file);
            if (result == 0) success = true;
        }
        else Log(2, "FILEIO: [%s] Failed to open file", fileName);
    }
    else Log(2, "FILEIO: File name provided is not valid");

    return success;
}

inline char *LoadFileText(const char *fileName)
{
    char *text = NULL;

    if (fileName != NULL)
    {
        SDL_RWops* textFile= SDL_RWFromFile(fileName, "rt");
        if (textFile != NULL)
        {
          unsigned  int size =(int) SDL_RWsize(textFile);
            if (size > 0)
            {
                text = (char *)malloc((size + 1)*sizeof(char));
                unsigned int count = (unsigned int) SDL_RWread(textFile, text, sizeof( char), size);
                if (count < size) text = (char*)realloc(text, count + 1);
                text[count] = '\0';

                 Log(0, "FILEIO: [%s] Text file loaded successfully", fileName);
            }
            else Log(2, "FILEIO: [%s] Failed to read text file", fileName);

            SDL_RWclose(textFile);
        }
        else Log(2, "FILEIO: [%s] Failed to open text file", fileName);
    }
    else Log(2, "FILEIO: File name provided is not valid");

    return text;
}

inline bool SaveFileText(const char *fileName, char *text)
{
    bool success = false;

    if (fileName != NULL)
    {
         SDL_RWops* file= SDL_RWFromFile(fileName, "wt");
        if (file != NULL)
        {
             size_t strLen = SDL_strlen( text );
	         int count = SDL_RWwrite( file, text, 1, strLen );
            if (count < 0) Log(3, "FILEIO: [%s] Failed to write text file", fileName);
            else Log(0, "FILEIO: [%s] Text file saved successfully", fileName);

            int result = SDL_RWclose(file);
            if (result == 0) success = true;
        }
        else Log(2, "FILEIO: [%s] Failed to open text file", fileName);
    }
    else Log(2, "FILEIO: File name provided is not valid");
    return success;
}




//----------------------------------------------------------------------------------
// Text strings management functions
//----------------------------------------------------------------------------------
inline const char *strprbrk(const char *s, const char *charset)
{
    const char *latestMatch = NULL;
    for (; s = strpbrk(s, charset), s != NULL; latestMatch = s++) { }
    return latestMatch;
}

inline unsigned int TextLength(const char *text)
{
    unsigned int length = 0; //strlen(text)
    if (text != NULL)
    {
        while (*text++) length++;
    }

    return length;
}

inline const char *TextFormat(const char *text, ...)
{

    #define MAX_TEXTFORMAT_BUFFERS 4        // Maximum number of static buffers for text formatting
    static char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = { 0 };
    static int  index = 0;
    char *currentBuffer = buffers[index];
    memset(currentBuffer, 0, MAX_TEXT_BUFFER_LENGTH);   // Clear buffer before using
    va_list args;
    va_start(args, text);
    vsprintf(currentBuffer, text, args);
    va_end(args);
    index += 1;     // Move to next buffer for next function call
    if (index >= MAX_TEXTFORMAT_BUFFERS) index = 0;

    return currentBuffer;
}

inline int TextToInteger(const char *text)
{
    int value = 0;
    int sign = 1;

    if ((text[0] == '+') || (text[0] == '-'))
    {
        if (text[0] == '-') sign = -1;
        text++;
    }
    for (int i = 0; ((text[i] >= '0') && (text[i] <= '9')); ++i) value = value*10 + (int)(text[i] - '0');
    return value*sign;
}



inline int TextCopy(char *dst, const char *src)
{
    int bytes = 0;

    if (dst != NULL)
    {
        while (*src != '\0')
        {
            *dst = *src;
            dst++;
            src++;

            bytes++;
        }

        *dst = '\0';
    }

    return bytes;
}

inline bool TextIsEqual(const char *text1, const char *text2)
{
    bool result = false;

    if (strcmp(text1, text2) == 0) result = true;

    return result;
}


inline const char *TextSubtext(const char *text, int position, int length)
{
    static char buffer[MAX_TEXT_BUFFER_LENGTH] = { 0 };

    int textLength = TextLength(text);

    if (position >= textLength)
    {
        position = textLength - 1;
        length = 0;
    }

    if (length >= textLength) length = textLength;

    for (int c = 0 ; c < length ; c++)
    {
        *(buffer + c) = *(text + position);
        text++;
    }

    *(buffer + length) = '\0';

    return buffer;
}

inline char *TextReplace(char *text, const char *replace, const char *by)
{
    // Sanity checks and initialization
    if (!text || !replace || !by) return NULL;

    char *result;

    char *insertPoint;      // Next insert point
    char *temp;             // Temp pointer
    int replaceLen;         // Replace string length of (the string to remove)
    int byLen;              // Replacement length (the string to replace replace by)
    int lastReplacePos;     // Distance between replace and end of last replace
    int count;              // Number of replacements

    replaceLen = TextLength(replace);
    if (replaceLen == 0) return NULL;  // Empty replace causes infinite loop during count

    byLen = TextLength(by);

    // Count the number of replacements needed
    insertPoint = text;
    for (count = 0; (temp = strstr(insertPoint, replace)); count++) insertPoint = temp + replaceLen;

    // Allocate returning string and point temp to it
    temp = result =(char*) malloc(TextLength(text) + (byLen - replaceLen)*count + 1);

    if (!result) return NULL;   // Memory could not be allocated

    // First time through the loop, all the variable are set correctly from here on,
    //    temp points to the end of the result string
    //    insertPoint points to the next occurrence of replace in text
    //    text points to the remainder of text after "end of replace"
    while (count--)
    {
        insertPoint = strstr(text, replace);
        lastReplacePos = (int)(insertPoint - text);
        temp = strncpy(temp, text, lastReplacePos) + lastReplacePos;
        temp = strcpy(temp, by) + byLen;
        text += lastReplacePos + replaceLen; // Move to next "end of replace"
    }

    // Copy remaind text part after replacement to result (pointed by moving temp)
    strcpy(temp, text);

    return result;
}


inline char *TextInsert(const char *text, const char *insert, int position)
{
    int textLen = TextLength(text);
    int insertLen =  TextLength(insert);

    char *result = (char *)malloc(textLen + insertLen + 1);

    for (int i = 0; i < position; i++) result[i] = text[i];
    for (int i = position; i < insertLen + position; i++) result[i] = insert[i];
    for (int i = (insertLen + position); i < (textLen + insertLen); i++) result[i] = text[i];

    result[textLen + insertLen] = '\0';     // Make sure text string is valid!

    return result;
}


inline const char *TextJoin(const char **textList, int count, const char *delimiter)
{
    static char text[MAX_TEXT_BUFFER_LENGTH] = { 0 };
    memset(text, 0, MAX_TEXT_BUFFER_LENGTH);
    char *textPtr = text;

    int totalLength = 0;
    int delimiterLen = TextLength(delimiter);

    for (int i = 0; i < count; i++)
    {
        int textLength = TextLength(textList[i]);

        // Make sure joined text could fit inside MAX_TEXT_BUFFER_LENGTH
        if ((totalLength + textLength) < MAX_TEXT_BUFFER_LENGTH)
        {
            memcpy(textPtr, textList[i], textLength);
            totalLength += textLength;
            textPtr += textLength;

            if ((delimiterLen > 0) && (i < (count - 1)))
            {
                memcpy(textPtr, delimiter, delimiterLen);
                totalLength += delimiterLen;
                textPtr += delimiterLen;
            }
        }
    }

    return text;
}



inline const char **TextSplit(const char *text, char delimiter, int *count)
{
    static const char *result[MAX_TEXTSPLIT_COUNT] = { NULL };
    static char buffer[MAX_TEXT_BUFFER_LENGTH] = { 0 };
    memset(buffer, 0, MAX_TEXT_BUFFER_LENGTH);

    result[0] = buffer;
    int counter = 0;

    if (text != NULL)
    {
        counter = 1;

        // Count how many substrings we have on text and point to every one
        for (int i = 0; i < MAX_TEXT_BUFFER_LENGTH; i++)
        {
            buffer[i] = text[i];
            if (buffer[i] == '\0') break;
            else if (buffer[i] == delimiter)
            {
                buffer[i] = '\0';   // Set an end of string at this point
                result[counter] = buffer + i + 1;
                counter++;

                if (counter == MAX_TEXTSPLIT_COUNT) break;
            }
        }
    }

    *count = counter;
    return result;
}


inline void TextAppend(char *text, const char *append, int *position)
{
    strcpy(text + *position, append);
    *position += TextLength(append);
}

inline int TextFindIndex(const char *text, const char *find)
{
    int position = -1;

    char *ptr = (char*)strstr(text, find);

    if (ptr != NULL) position = (int)(ptr - text);

    return position;
}


inline const char *TextToUpper(const char *text)
{
    static char buffer[MAX_TEXT_BUFFER_LENGTH] = { 0 };

    for (int i = 0; i < MAX_TEXT_BUFFER_LENGTH; i++)
    {
        if (text[i] != '\0')
        {
            buffer[i] = (char)toupper(text[i]);
        }
        else { buffer[i] = '\0'; break; }
    }

    return buffer;
}


inline const char *TextToLower(const char *text)
{
    static char buffer[MAX_TEXT_BUFFER_LENGTH] = { 0 };

    for (int i = 0; i < MAX_TEXT_BUFFER_LENGTH; i++)
    {
        if (text[i] != '\0')
        {
            buffer[i] = (char)tolower(text[i]);
        }
        else { buffer[i] = '\0'; break; }
    }

    return buffer;
}


inline const char *TextToPascal(const char *text)
{
    static char buffer[MAX_TEXT_BUFFER_LENGTH] = { 0 };

    buffer[0] = (char)toupper(text[0]);

    for (int i = 1, j = 1; i < MAX_TEXT_BUFFER_LENGTH; i++, j++)
    {
        if (text[j] != '\0')
        {
            if (text[j] != '_') buffer[i] = text[j];
            else
            {
                j++;
                buffer[i] = (char)toupper(text[j]);
            }
        }
        else { buffer[i] = '\0'; break; }
    }

    return buffer;
}

//----------------------------------------------------------------------------------
// Files management functions
//----------------------------------------------------------------------------------

inline bool FileExists(const char *fileName)
{
    bool result = false;
   if (access(fileName, F_OK) != -1) result = true;
    return result;
}

inline bool DirectoryExists(const char *dirPath)
{
    bool result = false;
    DIR *dir = opendir(dirPath);

    if (dir != NULL)
    {
        result = true;
        closedir(dir);
    }

    return result;
}

inline const char *GetFileExtension(const char *fileName)
{
    const char *dot = strrchr(fileName, '.');

    if (!dot || dot == fileName) return NULL;

    return (dot + 1);
}


inline const char *GetFileName(const char *filePath)
{
    const char *fileName = NULL;
    if (filePath != NULL) fileName = strprbrk(filePath, "\\/");

    if (!fileName) return filePath;

    return fileName + 1;
}



inline const char *GetFileNameWithoutExt(const char *filePath)
{
    #define MAX_FILENAMEWITHOUTEXT_LENGTH   128

    static char fileName[MAX_FILENAMEWITHOUTEXT_LENGTH];
    memset(fileName, 0, MAX_FILENAMEWITHOUTEXT_LENGTH);

    if (filePath != NULL) strcpy(fileName, GetFileName(filePath));   // Get filename with extension

    int len = (int)strlen(fileName);

    for (int i = 0; (i < len) && (i < MAX_FILENAMEWITHOUTEXT_LENGTH); i++)
    {
        if (fileName[i] == '.')
        {
            // NOTE: We break on first '.' found
            fileName[i] = '\0';
            break;
        }
    }

    return fileName;
}



inline const char *GetDirectoryPath(const char *filePath)
{
    const char *lastSlash = NULL;
    static char dirPath[MAX_FILEPATH_LENGTH];
    memset(dirPath, 0, MAX_FILEPATH_LENGTH);

    // In case provided path does not contain a root drive letter (C:\, D:\) nor leading path separator (\, /),
    // we add the current directory path to dirPath
    if (filePath[1] != ':' && filePath[0] != '\\' && filePath[0] != '/')
    {
        // For security, we set starting path to current directory,
        // obtained path will be concated to this
        dirPath[0] = '.';
        dirPath[1] = '/';
    }

    lastSlash = strprbrk(filePath, "\\/");
    if (lastSlash)
    {
        if (lastSlash == filePath)
        {
            // The last and only slash is the leading one: path is in a root directory
            dirPath[0] = filePath[0];
            dirPath[1] = '\0';
        }
        else
        {
            // NOTE: Be careful, strncpy() is not safe, it does not care about '\0'
            memcpy(dirPath + (filePath[1] != ':' && filePath[0] != '\\' && filePath[0] != '/' ? 2 : 0), filePath, strlen(filePath) - (strlen(lastSlash) - 1));
            dirPath[strlen(filePath) - strlen(lastSlash) + (filePath[1] != ':' && filePath[0] != '\\' && filePath[0] != '/' ? 2 : 0)] = '\0';  // Add '\0' manually
        }
    }

    return dirPath;
}


inline const char *GetPrevDirectoryPath(const char *dirPath)
{
    static char prevDirPath[MAX_FILEPATH_LENGTH];
    memset(prevDirPath, 0, MAX_FILEPATH_LENGTH);
    int pathLen = (int)strlen(dirPath);

    if (pathLen <= 3) strcpy(prevDirPath, dirPath);

    for (int i = (pathLen - 1); (i >= 0) && (pathLen > 3); i--)
    {
        if ((dirPath[i] == '\\') || (dirPath[i] == '/'))
        {
            // Check for root: "C:\" or "/"
            if (((i == 2) && (dirPath[1] ==':')) || (i == 0)) i++;

            strncpy(prevDirPath, dirPath, i);
            break;
        }
    }

    return prevDirPath;
}

inline const char *GetWorkingDirectory(void)
{
    static char currentDir[MAX_FILEPATH_LENGTH];
    memset(currentDir, 0, MAX_FILEPATH_LENGTH);

    char *ptr = GETCWD(currentDir, MAX_FILEPATH_LENGTH - 1);

    return ptr;
}



static char **dirFilesPath = NULL;          // Store directory files paths as strings
static int dirFilesCount = 0;               // Count directory files strings


inline void ClearDirectoryFiles(void)
{
    if (dirFilesCount > 0)
    {
        for (int i = 0; i < MAX_DIRECTORY_FILES; i++) free(dirFilesPath[i]);

        free(dirFilesPath);
    }

    dirFilesCount = 0;
}

inline char **GetDirectoryFiles(const char *dirPath, int *fileCount)
{
    #define MAX_DIRECTORY_FILES     512

    ClearDirectoryFiles();

    // Memory allocation for MAX_DIRECTORY_FILES
    dirFilesPath = (char **)malloc(sizeof(char *)*MAX_DIRECTORY_FILES);

    for (int i = 0; i < MAX_DIRECTORY_FILES; i++) dirFilesPath[i] = (char *)malloc(sizeof(char)*MAX_FILEPATH_LENGTH);

    int counter = 0;
    struct dirent *entity;
    DIR *dir = opendir(dirPath);

    if (dir != NULL)  // It's a directory
    {
           while ((entity = readdir(dir)) != NULL)
           {
            strcpy(dirFilesPath[counter], entity->d_name);
            counter++;
        }

        closedir(dir);
    }
    else Log(2, "FILEIO: Failed to open requested directory");  // Maybe it's a file...

    dirFilesCount = counter;
    *fileCount = dirFilesCount;

    return dirFilesPath;
}




inline bool ChangeDirectory(const char *dir)
{
    bool result = CHDIR(dir);

    if (result != 0) Log(2, "SYSTEM: Failed to change to directory: %s", dir);

    return (result == 0);
}


inline bool IsFileExtension(const char *fileName, const char *ext)
{
    bool result = false;
    const char *fileExt = GetFileExtension(fileName);

    if (fileExt != NULL)
    {

        int extCount = 0;
        const char **checkExts = TextSplit(ext, ';', &extCount);

        char fileExtLower[16] = { 0 };
        strcpy(fileExtLower, TextToLower(fileExt));

        for (int i = 0; i < extCount; i++)
        {
            if (TextIsEqual(fileExtLower, TextToLower(checkExts[i] + 1)))
            {
                result = true;
                break;
            }
        }
    }

    return result;
}
