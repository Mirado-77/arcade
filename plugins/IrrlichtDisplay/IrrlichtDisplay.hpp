/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** IrrlichtDisplay
*/

#ifndef IRRLICHTDISPLAY_HPP
    #define IRRLICHTDISPLAY_HPP

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wshadow"
    #pragma GCC diagnostic ignored "-Wdeprecated-copy"
    #include <irrlicht.h>
    #pragma GCC diagnostic pop

    #include <queue>
    #include <unordered_map>
    #include <vector>
    #include "IDisplayModule.hpp"

namespace arc {
    class IrrlichtEventReceiver : public irr::IEventReceiver {
        public:
            bool OnEvent(const irr::SEvent &event) override;
            arc::Event popEvent() noexcept;
            bool hasEvent() const noexcept;

        private:
            std::queue<arc::Event> _queue;

            static const std::unordered_map<irr::EKEY_CODE, arc::Action> KEY_MAP;
            static arc::Action keyCodeToAction(irr::EKEY_CODE code) noexcept;
    };

    class IrrlichtDisplay : public IDisplayModule {
        public:
            IrrlichtDisplay();
            ~IrrlichtDisplay() override;

            Event getEvent() noexcept override;
            int setAssets(const Assets assets) noexcept override;
            void drawGame(const std::pair<Entities, Sounds> data) noexcept override;

        private:
            // Window / FPS
            static constexpr irr::u32 WIN_W = 1280u;
            static constexpr irr::u32 WIN_H = 720u;
            static constexpr irr::u32 HUD_H = 30u;
            static constexpr irr::u32 TARGET_FPS = 60u;

            // 3D world grid (world units)
            static constexpr float GRID_W = 40.f;
            static constexpr float GRID_D = 20.f;
            static constexpr float BOX_H  = 0.8f;

            // Unit-box geometry: X/Z in [-0.5,0.5], Y in [0,1]
            static const irr::video::S3DVertex CUBE_VERTS[8];
            static const irr::u16 CUBE_IDX[36];

            IrrlichtEventReceiver _receiver;
            irr::IrrlichtDevice *_device;
            irr::video::IVideoDriver *_driver;
            irr::scene::ISceneManager *_smgr;
            irr::gui::IGUIEnvironment *_guienv;
            irr::scene::ICameraSceneNode *_camera;
            irr::u32 _lastFrameMs;
            std::vector<irr::video::ITexture *> _textures;

            void setupCamera() noexcept;
            void drawGround() noexcept;
            void drawHud() noexcept;
            void drawEntity(const std::unique_ptr<IEntity> &entity) noexcept;
            void drawBox3D(irr::video::SColor color,
                float cx, float cz,
                float w, float d,
                float rotY) noexcept;
            void limitFrameRate() noexcept;

            static irr::video::SColor toSColor(RGBA rgba) noexcept;
    };
}

#endif
