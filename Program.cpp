#include "CubeInsWindow.h"

using namespace std;

int main() {
    CubeInsWindow *pWnd = new CubeInsWindow(100, 100, 100);
    pWnd->run();
    delete pWnd;
    return 0;
}
