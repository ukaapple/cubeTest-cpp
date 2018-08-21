#include "CubeInsWindow.h"

using namespace std;

int main() {
    CubeInsWindow *pWnd = new CubeInsWindow(200, 200, 200);
    pWnd->run();
    delete pWnd;
    return 0;
}
