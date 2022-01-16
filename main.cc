#include <iostream>

#include "Graphics.hh"

int main()
{
    MYTH::GRAPHICS Graphics; // GRAPHICS OMG
    MYTH::TEXTBOX TextBox; // Creates a textbox

    int OS = MYTH::FUNCTIONS::init(); 
    // Initializes, you can get which operating system that is program running in
    // returns 1 if its windows
    // returns 2 if its linux

    MYTH::FUNCTIONS::clearConsole(); // Clears the terminal
    

    TextBox.x = 25;
    TextBox.y = 3;
    // Sets the position

    TextBox.text_color = BLUE; // Sets the text color
    TextBox.border_color = GRAY; // Sets the border color

    TextBox.border_auto = true; // Sets the border size automatically

    //TextBox.border_visible = false; <- Sets the border's visible
    
    /*
    TextBox.values["border_auto"] = FALSE;  <- to set the border size yourself
    TextBox.values["border_x"] = "46";
    TextBox.values["border_y"] = "2";
    */

    TextBox.text_aligned = true; // This makes the text aligned as you can see
    TextBox.text = "hello";
    
    MYTH::FUNCTIONS::Mouse::set_Visible(false); 
    // This function sets the terminal's cursor visible
    // and its windows only :c

    MYTH::FUNCTIONS::clearConsole(); // Clears the terminal
    Graphics.print_Textbox(TextBox); // Prints the textbox

    MYTH::FUNCTIONS::Mouse::set_Visible(true);
    return 0;
}