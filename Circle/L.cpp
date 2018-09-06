#include"L.h"
using std::cout;
using std::endl;
using std::string;

void L::v(string tag, string msg) {
	if (level <= VERBOSE) cout << "V_" << tag << "_" << msg << endl;
}
void L::d(string tag, string msg) {
	if (level <= DEBUG) cout << "D_" << tag << "_" << msg << endl;
}
void L::i(string tag, string msg) {
	if (level <= INFO) cout << "I_" << tag << "_" << msg << endl;
}
void L::w(string tag, string msg) {
	if (level <= WARN) cout << "!!!_" << tag << "_" << msg << endl;
}
void L::e(string tag, string msg) {
	if (level <= ERROR) cout << "ERR_" << tag << "_" << msg << endl;
}
unsigned int L::level = 1;