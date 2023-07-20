#include <cstdio>
#include <string>
#include <vector>

using std::byte;

std::byte operator""_B(char c)
{
  return static_cast<byte>(c);
}
std::byte operator""_B(unsigned long long c)
{
  return static_cast<byte>(c);
}

static const byte ESC{'H'};
static const byte SOF{0x10};

using CompleteCb = void (*)(std::string&);

void ProcessNextByte(byte b, CompleteCb frameCompleted)
{
  static std::string frame{};
  static bool inHeader{}, wasESC{}, lookingForSOF{};

  if(inHeader) {
    if((ESC == b) and not wasESC) {
      wasESC = true;
      return;
    } else if(wasESC) {
      wasESC = false;

      if((SOF == b) or (ESC != b)) {
        // if b is not SOF discard the frame
        if(SOF == b) { frameCompleted(frame); }
        frame.clear();
        inHeader = false;
        return;
      }
    }
    frame += static_cast<char>(b);

  } else if((ESC == b) and not lookingForSOF) {
    lookingForSOF = true;
  } else if((SOF == b) and lookingForSOF) {
    inHeader      = true;
    lookingForSOF = false;
  } else {
    lookingForSOF = false;
  }
}

int main()
{
  const std::vector<byte> fakeBytes1{0x70_B,
                                     0x24_B,
                                     ESC,
                                     SOF,
                                     ESC,
                                     'H'_B,
                                     'e'_B,
                                     'l'_B,
                                     'l'_B,
                                     'o'_B,
                                     ESC,
                                     SOF,
                                     0x7_B,
                                     ESC,
                                     SOF};

  auto frameCompleteHandler = [](std::string& res) {
    printf("CCCB: %s\n", res.c_str());
  };

  for(const auto& b : fakeBytes1) {
    ProcessNextByte(b, frameCompleteHandler);
  }

  printf("----------\n");

  const std::vector<byte> fakeBytes2{'W'_B,
                                     'o'_B,
                                     'r'_B,
                                     'l'_B,
                                     'd'_B,
                                     ESC,
                                     SOF,
                                     0x99_B};

  for(const auto& b : fakeBytes2) {
    ProcessNextByte(b, frameCompleteHandler);
  }
}