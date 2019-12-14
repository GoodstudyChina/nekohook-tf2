
#pragma once

namespace neko::hook::offsets {

#if defined(__linux__)
constexpr int CreateMove = 22;
constexpr int PaintTraverse = 42;
#elif defined(_WIN32)
constexpr int CreateMove = 21;
constexpr int PaintTraverse = 41;
#endif
constexpr int OverrideView = 17;
constexpr int FrameStageNotify = 35;
constexpr int SendNetMsg = 41;

}
