all:
	g++ main.cpp Engine.cpp Component.cpp System.cpp IOManager.cpp SettingsManager.cpp Setting.cpp WindowSystem.cpp RenderSystem.cpp ClickDragSystem.cpp registry.cpp ClickDragComponent.cpp TextMessageComponent.cpp RegComponent.cpp RegTexture.cpp RegTextureMapped.cpp MovementSystem.cpp NullComponent.cpp ColorComponent.cpp PositionComponent.cpp TextureComponent.cpp DimensionsComponent.cpp VectorComponent.cpp TimekeeperSystem.cpp DeltaMessage.cpp Entity.cpp libSOIL.a -lGLEW -lglfw -lGL -lGLU -lgdiplus -o rts -std=c++11

