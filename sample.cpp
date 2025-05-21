#include "Wav.hpp"
#include "MyWavProcessor.hpp" // Include the new processor

int main() {
    Wav wav;
    MyWavProcessor myProcessor; // Create an instance of MyWavProcessor

    wav.setProcessor(&myProcessor); // Set the processor on the Wav object

    // It's good practice to check if the file exists or can be opened.
    // For now, we'll assume "test2.wav" exists for demonstration.
    // The plan includes adding file error checks later.
    wav.open("test2.wav");

    return 0;
}
