#ifndef DATA_H
#define DATA_H

#include <switch.h>

#include <vector>
#include <string>

#include "gfx.h"

#define curUser users[data::selUser]
#define curData users[data::selUser].titles[data::selData]

namespace data
{
    extern bool forceMount;

    //Loads user + title info
    void init();
    void exit();
    bool loadUsersTitles(bool clearUsers);
    void loadBlacklist();
    void saveBlackList();
    void loadCfg();
    void saveCfg();
    void loadFav();
    void saveFav();

    //Class to help not load the same icons over and over
    class icn
    {
        public:
            icn() = default;
            //Loads jpeg icon from jpegData
            icn(const uint64_t& _id, const uint8_t *jpegData, const size_t& jpegSize);
            //Creates a generic icon for stuff with no icon with id
            icn(const uint64_t& _id, const std::string& _txt);

            //Creates favorite with heart on it. Needs to be called after icon is loaded
            void createFav();

            void draw(unsigned x, unsigned y) { texDrawNoAlpha(iconTex, frameBuffer, x, y); }
            void drawFav(unsigned x, unsigned y) { texDrawNoAlpha(iconFav, frameBuffer, x, y); }
            void drawHalf(unsigned x, unsigned y) { texDrawSkipNoAlpha(iconTex, frameBuffer, x, y); }
            void drawFavHalf(unsigned x, unsigned y) { texDrawSkipNoAlpha(iconFav, frameBuffer, x, y); }

            uint64_t getTitleID() { return titleID; }
            tex *getTex() { return iconTex; }

            void deleteData() { texDestroy(iconTex); texDestroy(iconFav); }

        private:
            uint64_t titleID;
            tex *iconTex, *iconFav = NULL;
    };

    //Class to store title info
    class titledata
    {
        public:
            titledata() = default;
            //Attempts to read title's info
            titledata(const FsSaveDataInfo& inf, NsApplicationControlData *dat);

            //Attempts to mount data with uID + id. Returns false if fails. For filtering.
            bool isMountable(const AccountUid& uid);

            //Returns title + title without forbidden chars
            std::string getTitle() { return title;}
            std::string getTitleSafe() { return titleSafe; }
            std::string getAuthor() { return author; }

            //Creates title folder
            void createDir();
            //Returns folder path
            std::string getPath() { return path; }

            //returns save_data_id string. only used for helping identify nand files
            std::string getTIDStr() { return tidStr; }
            std::string getSaveIDStr() { return saveIDStr; }

            uint64_t getID() { return id; }
            uint64_t getSaveID() { return saveID; }
            uint16_t getSaveIndex() { return saveIndex; }
            FsSaveDataType getType() { return (FsSaveDataType)saveDataType; }
            void setType(FsSaveDataType type) { saveDataType = type; }
            void setFav(bool setFav) { favorite = setFav; }
            bool getFav() { return favorite; }

            //Game icon
            icn icon;

        private:
            uint8_t saveDataType;
            std::string title, titleSafe, author, path, tidStr, saveIDStr;
            uint64_t id, saveID;
            uint16_t saveIndex;
            bool favorite = false;
    };

    //Class to store user info + titles
    class user
    {
        public:
            user() = default;
            user(const AccountUid& _id, const std::string& _backupName);
            user(const AccountUid& _id, const std::string& _backupName, tex *img);

            //Sets ID
            void setUID(const AccountUid& _id);

            //Assigns icon
            void assignIcon(tex *_icn) { userIcon = _icn; }

            //Returns user ID
            AccountUid getUID() { return userID; }
            u128 getUID128() { return uID128; }

            //Returns username
            std::string getUsername() { return username; }
            std::string getUsernameSafe() { return userSafe; }

            //Vector for storing save data info for user
            std::vector<titledata> titles;

            void drawIcon(int x, int y) { texDrawNoAlpha(userIcon, frameBuffer, x, y); }
            void drawIconHalf(int x, int y) { texDrawSkipNoAlpha(userIcon, frameBuffer, x, y); }
            void delIcon() { texDestroy(userIcon); }

        private:
            AccountUid userID;
            u128 uID128;
            std::string username, userSafe;
            //User icon
            tex* userIcon;
    };

    //Adds title to blacklist
    void blacklistAdd(data::titledata& t);
    //Adds title to favorite list
    void favoriteTitle(data::titledata& t);

    //User vector
    extern std::vector<icn> icons;
    extern std::vector<user> users;

    //Options and info
    extern int selUser, selData;
    extern SetLanguage sysLang;
    extern bool incDev, autoBack, ovrClk, holdDel, holdRest, holdOver, forceMount, accSysSave, sysSaveWrite, directFsCmd, skipUser;
}

#endif // DATA_H
