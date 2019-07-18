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
	   	  ShareUriWindow.h \
		  Client.h \	
		  AccountMessage.h \
		  DocumentMessage.h \
		  LoginMessage.h \
		  LogoutMessage.h \
		  Message.h \
		  Presence.h

SOURCES         = textedit.cpp \
                  main.cpp \
		  LandingPage.cpp \
		  ProfileEditWindow.cpp \
		  OpenUriWindow.cpp \
		  ShareUriWindow.cpp \
		  NewFileWindow.cpp \
		  Client.cpp \
		  Presence.cpp \
		  AccountMessage.cpp \
		  DocumentMessage.cpp \
		  LoginMessage.cpp \
		  LogoutMessage.cpp \
		  Message.cpp

FORMS 		= landingpage.ui \
		  profileeditwindow.ui \
		  openuriwindow.ui \
		  newfilewindow.ui \
		  shareuriwindow.ui

RESOURCES += textedit.qrc
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

EXAMPLE_FILES = textedit.qdoc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/richtext/textedit
INSTALLS += target