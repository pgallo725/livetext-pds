QT += widgets
QT += network websockets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

TEMPLATE        = app
TARGET          = textedit

HEADERS         = textedit.h \
		  LandingPage.h \
		  ProfileEditWindow.h \
		  OpenUriWindow.h \
		  NewFileWindow.h \
		  Client.h

SOURCES         = textedit.cpp \
                  main.cpp \
		  LandingPage.cpp \
		  ProfileEditWindow.cpp \
		  OpenUriWindow.cpp \
		  NewFileWindow.cpp \
		  Client.cpp

FORMS 		= landingpage.ui \
		  profileeditwindow.ui \
		  openuriwindow.ui \
		  newfilewindow.ui

RESOURCES += textedit.qrc
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

EXAMPLE_FILES = textedit.qdoc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/textedit
INSTALLS += target