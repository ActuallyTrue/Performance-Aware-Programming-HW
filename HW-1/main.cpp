#include <fstream>
#include <iterator>
#include <vector>


int main()
{
    std::ifstream input("listing_0037_single_register_mov", std::ios::binary);

    std::vector<char> bytes(
         (std::istreambuf_iterator<char>(input)),
         (std::istreambuf_iterator<char>()));

    input.close();
}