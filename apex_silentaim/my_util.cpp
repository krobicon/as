#pragma once
#include <vector>
#include <cfloat>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Math.cpp"
#include "Level.cpp"
#include "X11Utils.cpp"

class my_util
{
private:
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;
    X11Utils *m_x11Utils;
    
    long tmp_counter1;
    bool third_person_flag;
    
    long tmp_counter2;
    bool item_glow_flag;
public:
    my_util(
          Level *level,
          LocalPlayer *localPlayer,
          std::vector<Player *> *players,
          X11Utils *x11Utils)
    {
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
        m_x11Utils = x11Utils;
        
        tmp_counter1 = 0;
        third_person_flag = false;
        
        tmp_counter2 = 0;
        item_glow_flag = true;
    }
    
    void glow_enemy()
    {
    	char namebuf[64 + 1] = { 0 };
        memset(namebuf, 0, 65);
        
        bool ittisita_flag = false;
    
        if (!m_level->isPlayable())
            return;
        for (int i = 0; i < m_players->size(); i++)
        {
            Player *player = m_players->at(i);
            if (!player->isValid())
                continue;
            if (player->getTeamNumber() == m_localPlayer->getTeamNumber())
                continue;
            
            
        	if (player->isKnocked()) {
	       	 	player->setGlowEnable(1);
				player->setGlowThroughWall(2);
		 		player->setGlowType(101,101,46,90);
		 	    player->setGlowColor(FLT_MAX, 0.0f, 0.0f);//red
	       	}
            else if (player->isVisible())
            {
                player->setGlowEnable(1);
                player->setGlowThroughWall(2);
                player->setGlowType(101,101,46,90);
                player->setGlowColor(0.0f, 0.0f, FLT_MAX);//BLUE
            }
            else {
                player->setGlowEnable(1);
                player->setGlowThroughWall(2);
                player->setGlowType(101,101,46,90);
                player->setGlowColor(FLT_MAX / 2, 0.0f, FLT_MAX / 2);//PURPLE
            }
        }
    }
    
    void item_glow__pagedown(int counter) {
    	if (m_x11Utils->keyDown(0xff56) == true) {//PAGE_DOWN
             if (tmp_counter2 == 0) {
                  item_glow_flag = !item_glow_flag;
                  
                  if (item_glow_flag == true) {
                      printf("\a");//BEEP SOUND
                      fflush(stdout);
                      std::this_thread::sleep_for(std::chrono::milliseconds(200));
                      printf("\a");//BEEP SOUND
                      fflush(stdout);
                  }
                  else {
                      printf("\a");//BEEP SOUND
                      fflush(stdout);
                  }
             }
             tmp_counter2++;
        }
        else {
             tmp_counter2 = 0;
        }
    
        if (m_level->isPlayable() == true) {
		//for (size_t i = 0; i < 0x10000; i++) {
		    for (size_t i = 0; i < 15; i++) {
		        int index = (counter * 15) + i;
		        long ent_base = mem::ReadLong(offsets::REGION + offsets::ENTITY_LIST + ((index + 1) << 5));
		        if (ent_base == 0) {
		            continue;
		        }
		        
		        /*long ptrLong = ent_base + offsets::NAME;
		        std::string result = mem::ReadString(ptrLong);
		        if (result.empty()) {
		            continue;
		        }*/
		        std::string class_name = mem::get_client_class_name(ent_base);
		        //printf("class_name = %s\n", class_name.c_str());
		        if (class_name != "CPropSurvival" && class_name != "CWeaponX") {
			    	continue;
				}
		        
		        int itemID = mem::ReadInt(ent_base + 0x1648);
		        switch (itemID) {
		            case 1:   // Kraber
		            case 27:  // VK-47 Flatline
		            case 47:  // r99
		            case 57:  // volt
		            case 67:  // charge rifle
		            case 72:  // spitfire
		            case 77:  // R-301 Carbine
		            case 127: // rampage
		            case 128: // car
		            case 172: // Shield (Level 3 / Purple)
		            case 176: // Evo Shield (Level 3 / Purple)
		            case 177: // Evo Shield (Level 5 / red)
		            case 171: // Helmet (Level 4 / Gold)
		            case 184: //backpack level 2
		            case 185: // Backpack (Level 3 / Purple)
		            case 186: // Backpack (Level 4 / Gold)
		            case 167: // Head Level 3 / Purple
		            case 168: // Head Level 4 / Gold
		            case 163: // shield battery
		            case 210: //light mag level 3
		            case 211: //light mag level 4
		            case 214: //heavy mag level 3
		            case 215: //heavy mag level 4
		            case 222: //sniper mag level 3
		            case 223: //sniper mag level 4
		            case 182: //knockdown shield level 4
		                if (item_glow_flag == true) {
		                    mem::WriteInt(ent_base + 0x02c0, 1363184265);
		                }
		                else {
		                    mem::WriteInt(ent_base + 0x02c0, 0);
		                }
		                break;
		        }
		    }
        }
    }
    
    void invisible_my_weapon()
    {
    	if (m_level->isPlayable() == false) {
    		return;
    	}
    
        long MyLocalplayer = mem::ReadLong(offsets::REGION + offsets::LOCAL_PLAYER);

		long ViewModelHandle = mem::ReadLong(MyLocalplayer + offsets::OFFSET_ViewModels) & 0xFFFF; //m_hViewModels
		long ViewModelPtr = mem::ReadLong(offsets::REGION + offsets::ENTITY_LIST + ViewModelHandle * 0x20);
		    
		mem::WriteInt(ViewModelPtr + 0x3C8, 1);
		mem::WriteInt(ViewModelPtr + 0x3D0, 2);
		char w_buf[4] = { 101,101,46,0 };
		mem::writebytearray(ViewModelPtr + 0x2C4, w_buf, 4);
		mem::WriteFloat(ViewModelPtr + 0x1D0, 1.f);
		mem::WriteFloat(ViewModelPtr + 0x1D4, 19.f);
		mem::WriteFloat(ViewModelPtr + 0x1D8, 20.f);
    }
    
    void process_thirdperson__pageup() {
        if (m_x11Utils->keyDown(0xff55) == true) {//PAGE_UP
             if (tmp_counter1 == 0) {
                  third_person_flag = !third_person_flag;
             }
             tmp_counter1++;
        }
        else {
             tmp_counter1 = 0;
        }
        
        if (!m_level->isPlayable()) {
            return;
        }
    
        long MyLocalplayer = mem::ReadLong(offsets::REGION + offsets::LOCAL_PLAYER);
        
		if (third_person_flag == true) {
    		if (m_localPlayer->isZooming() == false/*m_x11Utils->keyDown(0xff55) == true*/) {
    	 	    mem::WriteInt(offsets::REGION + offsets::thirdperson_override + 0x6c, 1);
    		    mem::WriteInt(MyLocalplayer + offsets::m_thirdPersonShoulderView, 1);
    		}
    		else {
    		    mem::WriteInt(offsets::REGION + offsets::thirdperson_override + 0x6c, -1);
    		    mem::WriteInt(MyLocalplayer + offsets::m_thirdPersonShoulderView, -1);
    		}
    	}
    	else {
    		mem::WriteInt(offsets::REGION + offsets::thirdperson_override + 0x6c, -1);
    		mem::WriteInt(MyLocalplayer + offsets::m_thirdPersonShoulderView, -1);
    	}
    }
    
    void speed_down__insert() {
        if (!m_level->isPlayable()) {
            return;
        }
    
        if (m_x11Utils->keyDown(0xff63) == true) {//INSERT
            mem::WriteFloat(offsets::REGION + offsets::OFFSET_TIMESCALE + 0x68, std::numeric_limits<float>::min());
            //mem::WriteFloat(offsets::REGION + offsets::OFFSET_TIMESCALE + 0x68, 0.0f);
        }
        else {
            mem::WriteFloat(offsets::REGION + offsets::OFFSET_TIMESCALE + 0x68, 1.0f);
        }
    }
    
};
