/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** IrrlichtDisplay
*/

#include <cmath>
#include <memory>
#include <string>
#include "IrrlichtDisplay.hpp"

const std::unordered_map<irr::EKEY_CODE, arc::Action>
arc::IrrlichtEventReceiver::KEY_MAP = {
    {irr::KEY_UP,      Action::Up},
    {irr::KEY_DOWN,    Action::Down},
    {irr::KEY_LEFT,    Action::Left},
    {irr::KEY_RIGHT,   Action::Right},
    {irr::KEY_ESCAPE,  Action::Escape},
    {irr::KEY_F2,      Action::F2},
    {irr::KEY_F3,      Action::F3},
    {irr::KEY_F4,      Action::F4},
    {irr::KEY_F5,      Action::F5},
    {irr::KEY_RETURN,  Action::Enter},
    {irr::KEY_SPACE,   Action::Space},
    {irr::KEY_BACK,    Action::Backspace},
    {irr::KEY_KEY_A,   Action::A},
    {irr::KEY_KEY_B,   Action::B},
    {irr::KEY_KEY_C,   Action::C},
    {irr::KEY_KEY_D,   Action::D},
    {irr::KEY_KEY_E,   Action::E},
    {irr::KEY_KEY_F,   Action::F},
    {irr::KEY_KEY_G,   Action::G},
    {irr::KEY_KEY_H,   Action::H},
    {irr::KEY_KEY_I,   Action::I},
    {irr::KEY_KEY_J,   Action::J},
    {irr::KEY_KEY_K,   Action::K},
    {irr::KEY_KEY_L,   Action::L},
    {irr::KEY_KEY_M,   Action::M},
    {irr::KEY_KEY_N,   Action::N},
    {irr::KEY_KEY_O,   Action::O},
    {irr::KEY_KEY_P,   Action::P},
    {irr::KEY_KEY_Q,   Action::Q},
    {irr::KEY_KEY_R,   Action::R},
    {irr::KEY_KEY_S,   Action::S},
    {irr::KEY_KEY_T,   Action::T},
    {irr::KEY_KEY_U,   Action::U},
    {irr::KEY_KEY_V,   Action::V},
    {irr::KEY_KEY_W,   Action::W},
    {irr::KEY_KEY_X,   Action::X},
    {irr::KEY_KEY_Y,   Action::Y},
    {irr::KEY_KEY_Z,   Action::Z},
    {irr::KEY_KEY_0,   Action::Num0},
    {irr::KEY_KEY_1,   Action::Num1},
    {irr::KEY_KEY_2,   Action::Num2},
    {irr::KEY_KEY_3,   Action::Num3},
    {irr::KEY_KEY_4,   Action::Num4},
    {irr::KEY_KEY_5,   Action::Num5},
    {irr::KEY_KEY_6,   Action::Num6},
    {irr::KEY_KEY_7,   Action::Num7},
    {irr::KEY_KEY_8,   Action::Num8},
    {irr::KEY_KEY_9,   Action::Num9},
};

arc::Action arc::IrrlichtEventReceiver::keyCodeToAction(
    irr::EKEY_CODE code) noexcept
{
    auto it = KEY_MAP.find(code);
    return it != KEY_MAP.end() ? it->second : Action::None;
}

bool arc::IrrlichtEventReceiver::OnEvent(const irr::SEvent &event)
{
    if (event.EventType == irr::EET_KEY_INPUT_EVENT
        && event.KeyInput.PressedDown) {
        Action act = keyCodeToAction(event.KeyInput.Key);
        if (act != Action::None)
            _queue.push({act, {0.f, 0.f}});
    }
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
            _queue.push({Action::LeftMouse,
                {static_cast<float>(event.MouseInput.X),
                    static_cast<float>(event.MouseInput.Y)}});
        if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
            _queue.push({Action::RightMouse,
                {static_cast<float>(event.MouseInput.X),
                    static_cast<float>(event.MouseInput.Y)}});
    }
    return false;
}

arc::Event arc::IrrlichtEventReceiver::popEvent() noexcept
{
    arc::Event ev = _queue.front();
    _queue.pop();
    return ev;
}

bool arc::IrrlichtEventReceiver::hasEvent() const noexcept
{
    return !_queue.empty();
}

// Unit box: X/Z in [-0.5, 0.5], Y in [0, 1]
// Ik its a bit weird but it makes the math easier when we scale/rotate/translate
// 0 = (-0.5, 0, -0.5)  1 = (0.5, 0, -0.5)
// 2 = ( 0.5, 0,  0.5)  3 = (-0.5, 0, 0.5)
// 4 = (-0.5, 1, -0.5)  5 = (0.5, 1, -0.5)
// 6 = ( 0.5, 1,  0.5)  7 = (-0.5, 1, 0.5)

// Cube vertex data (position, normal, color, texcoord)
const irr::video::S3DVertex arc::IrrlichtDisplay::CUBE_VERTS[8] = {
    {-0.5f, 0.f, -0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 0,0},
    { 0.5f, 0.f, -0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 1,0},
    { 0.5f, 0.f, 0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 1,1},
    {-0.5f, 0.f, 0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 0,1},
    {-0.5f, 1.f, -0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 0,0},
    { 0.5f, 1.f, -0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 1,0},
    { 0.5f, 1.f, 0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 1,1},
    {-0.5f, 1.f, 0.5f, 0,1,0, irr::video::SColor(255,255,255,255), 0,1},
};

//fucking triangles
const irr::u16 arc::IrrlichtDisplay::CUBE_IDX[36] = {
    0,1,5, 0,5,4, // front (-z)
    2,3,7, 2,7,6, // back (+z)
    3,0,4, 3,4,7, // left (-x)
    1,2,6, 1,6,5, // right (+x)
    4,5,6, 4,6,7, // top (+y)
    3,2,1, 3,1,0, // bottom (-y)
};

arc::IrrlichtDisplay::IrrlichtDisplay()
    : _device(nullptr), _driver(nullptr),
      _smgr(nullptr), _guienv(nullptr),
      _camera(nullptr), _lastFrameMs(0)
{
    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_OPENGL;
    params.WindowSize = {WIN_W, WIN_H};
    params.Bits = 32;
    params.Fullscreen = false;
    params.Stencilbuffer = true;
    params.Vsync = false;
    params.EventReceiver = &_receiver;
    _device = irr::createDeviceEx(params);
    if (!_device)
        return;
    _device->setWindowCaption(L"Arcade Irrlicht 3D");
    _driver = _device->getVideoDriver();
    _smgr = _device->getSceneManager();
    _guienv = _device->getGUIEnvironment();
    setupCamera();
    _lastFrameMs = _device->getTimer()->getTime();
}

arc::IrrlichtDisplay::~IrrlichtDisplay()
{
    if (!_device)
        return;
    _device->closeDevice();
    _device->run();
    _device->drop();
}

// iso camera setup: fixed position and targetc
void arc::IrrlichtDisplay::setupCamera() noexcept
{
    irr::core::vector3df pos(GRID_W * 0.5f, 22.f, -7.f);
    irr::core::vector3df target(GRID_W * 0.5f, 0.f, GRID_D * 0.6f);
    _camera = _smgr->addCameraSceneNode(nullptr, pos, target);
    _camera->setFOV(irr::core::degToRad(65.f));
    float aspect = static_cast<float>(WIN_W)
        / static_cast<float>(WIN_H - HUD_H);
    _camera->setAspectRatio(aspect);
}

arc::Event arc::IrrlichtDisplay::getEvent() noexcept
{
    if (!_device || !_device->run())
        return {Action::Close, {0.f, 0.f}};
    if (_receiver.hasEvent())
        return _receiver.popEvent();
    return {Action::None, {0.f, 0.f}};
}

int arc::IrrlichtDisplay::setAssets(const Assets assets) noexcept
{
    if (!_driver)
        return -1;
    _textures.clear();
    _driver->removeAllTextures();
    for (const auto &path : assets.first) {
        std::string fullPath = path + ".png";
        _textures.push_back(_driver->getTexture(fullPath.c_str()));
    }
    return 0;
}

irr::video::SColor arc::IrrlichtDisplay::toSColor(arc::RGBA rgba) noexcept
{
    auto [r, g, b, a] = rgba;
    return irr::video::SColor(a, r, g, b);
}

void arc::IrrlichtDisplay::limitFrameRate() noexcept
{
    constexpr irr::u32 FRAME_MS = 1000u / TARGET_FPS;
    irr::u32 elapsed = _device->getTimer()->getTime() - _lastFrameMs;
    if (elapsed < FRAME_MS)
        _device->sleep(FRAME_MS - elapsed);
    _lastFrameMs = _device->getTimer()->getTime();
}

void arc::IrrlichtDisplay::drawBox3D(irr::video::SColor color,
    float cx, float cz, float w, float d, float rotY) noexcept
{
    irr::video::S3DVertex verts[8];
    for (int i = 0; i < 8; i++) {
        verts[i] = CUBE_VERTS[i];
        verts[i].Color = color;
    }
    irr::core::matrix4 matS, matR, matT;
    matS.setScale({w, BOX_H, d});
    matR.setRotationDegrees({0.f, rotY, 0.f});
    matT.setTranslation({cx, 0.f, cz});
    _driver->setTransform(irr::video::ETS_WORLD, matT * matR * matS);
    irr::video::SMaterial mat;
    mat.Lighting = false;
    mat.BackfaceCulling = false;
    mat.ZWriteEnable = true;
    mat.ZBuffer = irr::video::ECFN_LESSEQUAL;
    mat.MaterialType = irr::video::EMT_SOLID;
    _driver->setMaterial(mat);
    _driver->drawVertexPrimitiveList(verts, 8, CUBE_IDX, 12,
        irr::video::EVT_STANDARD,
        irr::scene::EPT_TRIANGLES,
        irr::video::EIT_16BIT);
}

void arc::IrrlichtDisplay::drawGround() noexcept
{
    irr::video::SColor groundColor(255, 25, 25, 35);
    irr::video::S3DVertex verts[4] = {
        {0.f, -0.02f, 0.f, 0,1,0, groundColor, 0,0},
        {GRID_W, -0.02f, 0.f, 0,1,0, groundColor, 1,0},
        {GRID_W, -0.02f, GRID_D, 0,1,0, groundColor, 1,1},
        {0.f, -0.02f, GRID_D, 0,1,0, groundColor, 0,1},
    };
    irr::u16 idx[6] = {0, 1, 2, 0, 2, 3};
    irr::core::matrix4 id;
    _driver->setTransform(irr::video::ETS_WORLD, id);
    irr::video::SMaterial mat;
    mat.Lighting = false;
    mat.BackfaceCulling = false;
    mat.ZWriteEnable = true;
    mat.ZBuffer = irr::video::ECFN_LESSEQUAL;
    mat.MaterialType = irr::video::EMT_SOLID;
    _driver->setMaterial(mat);
    _driver->drawVertexPrimitiveList(verts, 4, idx, 2,
        irr::video::EVT_STANDARD,
        irr::scene::EPT_TRIANGLES,
        irr::video::EIT_16BIT);
}

void arc::IrrlichtDisplay::drawEntity(
    const std::unique_ptr<IEntity> &entity) noexcept
{
    auto [fpx, fpy] = entity->getPos();
    auto [fsw, fsh] = entity->getSize();
    float w = std::max(0.05f, fsw * GRID_W);
    float d = std::max(0.05f, fsh * GRID_D);
    float cx = (fpx + fsw * 0.5f) * GRID_W;
    float cz = (1.0f - fpy - fsh * 0.5f) * GRID_D;
    drawBox3D(toSColor(entity->getColor()), cx, cz, w, d,
        entity->getRotation());
}

void arc::IrrlichtDisplay::drawHud() noexcept
{
    irr::video::SColor bg(255, 50, 50, 50);
    _driver->draw2DRectangle(bg, irr::core::rect<irr::s32>(
        0, 0,
        static_cast<irr::s32>(WIN_W),
        static_cast<irr::s32>(HUD_H)));
    irr::gui::IGUIFont *font = _guienv->getBuiltInFont();
    if (!font)
        return;
    font->draw(
        L"[Irrlicht 3D]  F2 = NextDisplay  F3 = NextGame  F4 = Restart  ESC = Quit",
        irr::core::rect<irr::s32>(5, 8,
            static_cast<irr::s32>(WIN_W) - 5,
            static_cast<irr::s32>(HUD_H)),
        irr::video::SColor(255, 255, 255, 255));
}

// Apply camera transforms to the driver then draw the 3D scene
// 2D HUD overlay (does not need world transforms)
void arc::IrrlichtDisplay::drawGame(
    const std::pair<Entities, Sounds> data) noexcept
{
    if (!_driver)
        return;
    const auto &[entities, sounds] = data;
    _driver->beginScene(true, true, irr::video::SColor(255, 10, 10, 20));
    _smgr->drawAll();
    drawGround();
    for (const auto &entity : entities)
        drawEntity(entity);
    drawHud();
    _driver->endScene();
    limitFrameRate();
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif
extern "C"
{
    std::unique_ptr<arc::IDisplayModule> makeInstance()
    {
        return std::make_unique<arc::IrrlichtDisplay>();
    }
    arc::LibType getLibType()
    {
        return arc::LibType::Display;
    }
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
