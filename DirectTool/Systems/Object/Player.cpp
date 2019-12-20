#include "stdafx.h"
#include "Player.h"



Player::Player(string name, D3DXVECTOR2 pos, D3DXVECTOR2 size, ObjectType type, Pivot p)
	: GameObject(name, pos, size, type, p)
{
	currentMusic = nullptr;
	bPlay = false;
	select = -1;
	current = -2;
	soundValue = 1.0f;



	addSoundFunc = [=](wstring str) {
		string key = String::WStringToString(Path::GetFileNameWithoutExtension(str));
		SOUNDMANAGER->AddSound(key, String::WStringToString(str), true, true);

		wstring imageName = Path::GetFilePathWithoutExtension(str);
		imageName += L".png";

		Music* music;
		music = new Music;
		music->Name = key;
		music->TitleImage = new Texture(imageName);
		
		files.push_back(music);
	};


}


Player::~Player()
{
}

void Player::Init()
{
}

void Player::Release()
{
}

void Player::Update(float tick)
{
}

void Player::ImguiRender()
{
	ImGuiWindowFlags flag = 0;
	flag |= ImGuiWindowFlags_NoTitleBar;
	flag |= ImGuiWindowFlags_AlwaysAutoResize;
	static bool open = true;
	ImGui::SetNextWindowSize(ImVec2(300, 600));
	ImGui::SetNextWindowPos(ImVec2(WinSizeX / 2 - 150, WinSizeY / 2 - 300));
	ImGui::Begin("MP3Player",&open,flag);
	{

		if(currentMusic != nullptr)
			ImGui::Image(currentMusic->TitleImage->GetSRV(), ImVec2(ImGui::GetWindowSize().x, 200));
		else
			ImGui::Image(nullptr, ImVec2(ImGui::GetWindowSize().x, 200));
			
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PushItemWidth(ImGui::GetWindowSize().x-15);

		static float pos = 0.f;
		static float len = 100.f;

		static function<UINT(UINT)> GetMin = [](UINT uTime)->UINT {
			UINT time = uTime;
			time *= 0.001f;
			time /= 60;
			return time;
		};
		static function<UINT(UINT)> GetSec = [](UINT uTime)->UINT {
			UINT time = uTime;
			time *= 0.001f;
			time %= 60;
			return time;
		};

		if (currentMusic != nullptr)
		{

			len = SOUNDMANAGER->Getlength(currentMusic->Name);
			pos = SOUNDMANAGER->Getposition(currentMusic->Name);
		}

		string timeStr;
		UINT min = GetMin(pos);
		UINT sec = GetSec(pos);

		timeStr += to_string(min);
		timeStr += " : ";
		if (sec < 10)
			timeStr += "0";
		timeStr += to_string(sec);

		ImGui::ProgressBar(pos / len, ImVec2(0, 0), timeStr.c_str());
		if (ImGui::SliderFloat("", &pos, 0, len, ""))
		{
			if (currentMusic != nullptr)
				SOUNDMANAGER->Setposition(currentMusic->Name, pos);
		}


		if (ImGui::Button("Prev", ImVec2(50, 30)))
		{
			if (currentMusic != nullptr)
			{
				if (current == files.size() - 1)
				{
					SOUNDMANAGER->Stop(currentMusic->Name);
					current = 0;
					currentMusic = files[current];
					SOUNDMANAGER->Play(currentMusic->Name);
				}
				else
				{
					SOUNDMANAGER->Stop(currentMusic->Name);
					current++;
					currentMusic = files[current];
					SOUNDMANAGER->Play(currentMusic->Name);
				}
			}
			else
			{
				if (files.size() > 0)
				{
					current = 0;
					currentMusic = files[current];
					SOUNDMANAGER->Play(currentMusic->Name);
				}
			}

		} ImGui::SameLine();

		if (bPlay == false)
		{
			if (ImGui::Button("Play", ImVec2(50, 30)))
			{
				if (currentMusic != nullptr)
				{
					SOUNDMANAGER->Resume(currentMusic->Name);
					bPlay = true;
				}
				else
				{
					if (files.size() > 0)
					{
						current = Math::Random(0, files.size() - 1);
						currentMusic = files[current];
						SOUNDMANAGER->Play(currentMusic->Name);
						bPlay = true;
					}
				}
			}
		}
		else
		{
			if (ImGui::Button("Pause", ImVec2(50, 30)))
			{
				SOUNDMANAGER->Pause(currentMusic->Name);
				bPlay = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Next", ImVec2(50, 30)))
		{
			if (currentMusic != nullptr)
			{
				if (current == 0)
				{
					SOUNDMANAGER->Stop(currentMusic->Name);
					current = files.size()-1;
					currentMusic = files[current];
					SOUNDMANAGER->Play(currentMusic->Name);
				}
				else
				{
					SOUNDMANAGER->Stop(currentMusic->Name);
					current--;
					currentMusic = files[current];
					SOUNDMANAGER->Play(currentMusic->Name);
				}
			}
			else
			{
				if (files.size() > 0)
				{
					current = files.size()-1;
					currentMusic = files[current];
					SOUNDMANAGER->Play(currentMusic->Name);
				}
			}
		}

		//º¼·ý
		ImGui::SameLine();
		ImGui::BeginChild("Volume", ImVec2(130, 50), true, ImGuiWindowFlags_None);
		if (ImGui::SliderFloat("Volume", &soundValue, 0, 1))
		{
			if(currentMusic != nullptr)
				SOUNDMANAGER->SetVolume(currentMusic->Name, soundValue);
		}
		ImGui::EndChild();
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Volume");
			ImGui::EndTooltip();
		}

		if (ImGui::Button("Add", ImVec2(60, 20)))
		{
			D3DDesc desc;
			pRenderer->GetDesc(&desc);
			Path::OpenFileDialog(L"", Path::Mp3Filter, L"../_Resources/Sound/mp3", addSoundFunc, desc.Handle);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Add .mp3 file and same name png file");
			ImGui::EndTooltip();
		}

		
		ImGui::BeginChild("MusicList",ImVec2(ImGui::GetWindowSize().x-30, 200), true, ImGuiWindowFlags_None);
		{
			
			for (int n = 0; n < files.size(); n++)
			{
				bool is_selected = (select == n);
				if (ImGui::Selectable(files[n]->Name.c_str(), is_selected))
				{
					select = n;
					current = n;
					ImGui::OpenPopup("my_select_popup");
				}

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					if(files[n]->TitleImage != nullptr)
						ImGui::Image(files[n]->TitleImage->GetSRV(),ImVec2(100,100));
					ImGui::EndTooltip();
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::SameLine();

			if (ImGui::BeginPopup("my_select_popup"))
			{
				if (ImGui::Button("Play"))
				{
					if (bPlay)
					{
						bPlay = false;
						SOUNDMANAGER->Stop(currentMusic->Name);
					}

					currentMusic = files[current];
					SOUNDMANAGER->Play(currentMusic->Name);
					bPlay = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete"))
				{
					if (bPlay)
					{
						bPlay = false;
						SOUNDMANAGER->Stop(files[current]->Name);
						pos = 0;
						len = 100;
					}
					currentMusic = nullptr;
					SafeDelete(files[current]->TitleImage);
					files.erase(files.begin() + current);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

		}
		ImGui::EndChild();
	}
	ImGui::End();
}
