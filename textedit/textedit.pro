QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

TEMPLATE        = app
TARGET          = textedit

HEADERS         = textedit.h \
		  WelcomeWindow.h \
		  ProfileEditWindow.h

SOURCES         = textedit.cpp \
                  main.cpp \
		  WelcomeWindow.cpp \
		  ProfileEditWindow.cpp

FORMS 		= welcomewindow.ui \
		  profileeditwindow.ui

RESOURCES += textedit.qrc
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

EXAMPLE_FILES = textedit.qdoc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/textedit
INSTALLS += target