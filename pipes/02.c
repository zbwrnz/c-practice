#include <unistd.h>

int main(int argc, char * argv[]){
    execlp("ps", "ps", "ax", 0);
    return 0;
}
