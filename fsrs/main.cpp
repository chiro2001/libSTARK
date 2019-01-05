#include <iostream>
#include <fstream>
#include <string>
#include <languages/Bair/BairWitnessChecker.hpp>

#include <protocols/protocol.hpp>
#include "Fsrs.hpp"
#include "Fsrs_wrapper.hpp"

using namespace simple_fsrs;
using namespace simple_fsrs::ACSP_FOR_FSRS;

using std::cout;
using std::endl;
using std::string;
using std::stoul;

using std::vector;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        cout << "please pass 2 arguments." << endl;
        return 0;
    }

    const unsigned int a_num(stoul(argv[1]));
    const unsigned int b_num(stoul(argv[2]));
    unsigned int securityParameter = 60;

    execute(a_num, b_num, securityParameter);
    return 0;
}