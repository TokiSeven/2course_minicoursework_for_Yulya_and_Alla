SOURCES += \
    main.cpp \
    class_tile.cpp \
    class_map.cpp \
    class_player.cpp \
    class_unit.cpp \
    class_enemy.cpp

LIBS += -LC:/SFML/lib

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += C:/SFML/include
DEPENDPATH += C:/SFML/include

HEADERS += \
    class_tile.h \
    class_map.h \
    class_player.h \
    class_unit.h \
    class_enemy.h
