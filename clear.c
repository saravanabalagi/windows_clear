#include <windows.h>

int main(void){
    HANDLE hStdout; 
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
    COORD destinationPoint;
    SMALL_RECT sourceArea;
    CHAR_INFO Fill;
    SHORT delta, end;

    // Get console handle
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);     

    // Retrieve console information
    if (GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) {

        // How many lines needs the window to be moved to be clear
        delta = (csbiInfo.dwCursorPosition.Y) - csbiInfo.srWindow.Top;

        // Where the bottom of the window will fall after moving
        end = csbiInfo.srWindow.Bottom + delta;

        // If the window get out of the console buffer, it is necessary to scroll the buffer
        if (end >= csbiInfo.dwSize.Y){
            // Select all the console buffer as source
            sourceArea.Top = 0;
            sourceArea.Left = 0;
            sourceArea.Bottom = csbiInfo.dwSize.Y-1;
            sourceArea.Right = csbiInfo.dwSize.X-1;

            // Select the target point for the movement
            destinationPoint.X = 0;
            destinationPoint.Y = 0 - delta ;

            // Configure fill character and attributes for the empty area
            Fill.Char.AsciiChar = ' ';
            Fill.Attributes =  csbiInfo.wAttributes;

            // Scroll the buffer and init the end zone
            ScrollConsoleScreenBuffer( hStdout, &sourceArea, NULL, destinationPoint, &Fill);

            // Adjust new cursor position
            destinationPoint.X = 0;
            destinationPoint.Y = csbiInfo.dwSize.Y - (csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1);

        } else {
            // No buffer scroll is needed. Adjust the new cursor position
            destinationPoint.X = 0;
            destinationPoint.Y = csbiInfo.dwCursorPosition.Y + 1;
        }

        // In any case, the visible window needs to be moved depending on the new cursor position
        sourceArea.Top = destinationPoint.Y;
        sourceArea.Left = destinationPoint.X;
        sourceArea.Bottom = destinationPoint.Y + (csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1) -1 ;
        sourceArea.Right = csbiInfo.dwSize.X-1;

        // Place the visible window in the required place over the buffer
        SetConsoleWindowInfo(hStdout, TRUE, &sourceArea);

        // Place the cursor in its final position
        SetConsoleCursorPosition( hStdout, destinationPoint );
    }

    return 0;
}