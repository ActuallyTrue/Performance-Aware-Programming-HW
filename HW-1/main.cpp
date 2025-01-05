#include <fstream>

const char* shortArray[8] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
const char* wideArray[8]= { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };

void AddASMInstruction(unsigned char instruction_value, std::ofstream* output)
{
    if ((instruction_value & 0b111111000) == 0b10001000)
    {
        output->write("mov ", 4);
    }
}

void AddModRegRM(unsigned char instruction_value, std::ofstream* output, bool isRegisterDestination, bool using16BitRegister)
{
    if ((instruction_value & 0b11000000) == 0b11000000)
    {
        //this is a register to register move!
    }

    uint16_t register_name = (instruction_value >> 3) & 0b00000111;
    uint16_t RM_name = instruction_value & 0b00000111;
    if (isRegisterDestination)
    {
        output->write(using16BitRegister ? wideArray[register_name]
            : shortArray[register_name], 2);
        output->write(", ", 2);
        output->write(using16BitRegister ? wideArray[RM_name]
            : shortArray[RM_name], 2);
    }
    else
    {
        output->write(using16BitRegister ? wideArray[RM_name]
            : shortArray[RM_name], 2);
        output->write(", ", 2);
        output->write(using16BitRegister ? wideArray[register_name]
            : shortArray[register_name], 2);
    }
}

void decodeFile(const char* filename, const char* outputFileName)
{
    std::ifstream input(filename, std::ios::binary);
    std::ofstream output(outputFileName, std::ios::out);

    if (input && output)
    {
        output.write("bits 16\n", 8);
        // get length of file:
        input.seekg (0, input.end);
        int length = input.tellg();
        input.seekg (0, input.beg);

        // allocate memory:
        uint16_t* buffer = new uint16_t [length / 2];

        // read data as a block:
        input.read (reinterpret_cast<char*>(buffer),length);

        for (int i = 0; i < length / 2; i++)
        {
            uint16_t value = buffer[i];
            //turns out my machine is little endian, so I'm just swapping it to be big endian here.
            value = (value>>8) | (value<<8);
            unsigned char instruction = (value >> 8) & 0b11111111;
            AddASMInstruction(instruction, &output);
            bool isRegisterDestination = value & (1 << 9);
            bool using16BitRegister = value & (1 << 8);
            instruction = value & 0xFF;
            AddModRegRM(instruction, &output, isRegisterDestination, using16BitRegister);
            output.write("\n", 1);
        }

        input.close();
        output.close();

        delete[] buffer;
    }
    input.close();
    output.close();
}

int main()
{
    decodeFile("listing_0037_single_register_mov", "decode1.asm");
    decodeFile("listing_0038_many_register_mov", "decode2.asm");
}