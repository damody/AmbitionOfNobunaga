
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

std::map<std::string, int> TextMapping;

int main(int argc, char **argv)
{
	// 轉魔獸 w3e地圖 
	if (argc == 3)
	{
		cv::Mat pic;
		pic.create(161, 161, CV_32F);
		std::ifstream w3efile(argv[1], std::ifstream::binary);
		w3efile.seekg(0, w3efile.end);
		int FileSize = w3efile.tellg();
		w3efile.seekg(0, w3efile.beg);
		// allocate memory:
		char * Buffer = new char[FileSize];
		// read data as a block:
		w3efile.read(Buffer, FileSize);
		w3efile.close();

		//Set the counter to 0
		//and use it for the 
		int Counter = 0;
		int PointCounter = 0;
		for (int i = 69; i < FileSize; i++)
		{
			switch (Counter)
			{
				case 1:
				{
					//printf("1 %d\n", Buffer[i]);
					//I don't now why, but if it wasn't here, the editor would get an error ;)
					if (PointCounter / 161 > 160)
					{
						goto exitnow;
					}
					if (Buffer[i]-64 > 0)
					{
						Buffer[i] -= 64;
					}
					pic.at<float>(PointCounter/161, PointCounter%161) = Buffer[i];
					Counter++;
					break;
				}
				case 0:
				{
					//printf("0 %d\n", Buffer[i]);
					PointCounter++;
					break;
				}
				default:
				{
					break;
				}
			}
			if (Counter < 7)
			{
				Counter++;
			}
			else
			{
				Counter = 0;
			}
		}
	exitnow:
		for (int i = 0; i < 6; ++i)
		{
			pic.at<cv::Vec3b>(0, i) = pic.at<cv::Vec3b>(1, i);
		}
		cv::resize(pic.clone(), pic, cv::Size(1271, 1271), 0, 0, cv::INTER_CUBIC);
		normalize(pic.clone(), pic, 100, 150, cv::NORM_MINMAX, CV_8UC3);
		int iblur = atoi(argv[2]);
		if (iblur % 2 == 0)
		{
			iblur++;
		}
		for (int i = 1; i <= iblur-1; i = i + 2)
		{
			cv::blur(pic.clone(), pic, cv::Size(i, i));
		}
		for (int i = 1; i <= 3; ++i)
		{
			cv::blur(pic.clone(), pic, cv::Size(iblur, iblur));
		}
		cv::imwrite("war3.png", pic);
		return 0;
	} else if (argc < 8)
	{
		printf("輸入格式為\n"
			"convert_csv_to_heightmap.exe \"CSV路徑\" \"顏色對應表路徑\" \"輸出檔名.png\" 長 寬 模糊大小 模糊次數 模糊係數(1~100) \n"
			"convert_csv_to_heightmap.exe map_1.csv colormap.txt map_1.png 1271 1271 5 10 30\n");
		return 0;
	}
	
	// 得到csv長寬
	int csv_w=0, csv_h=0;
	std::ifstream csvfile(argv[1]);
	for (; csvfile.good();)
	{
		csv_h++;
		char line[65536] = { 0 };
		csvfile.getline(line, 65536);
		int nw = 0;
		for (int pos=0;line[pos];pos++)
		{
			if (line[pos] == ',')
			{
				nw++;
			}
		}
		if (nw > csv_w)
		{
			csv_w = nw;
		}
	}
	csv_h -= 2;
	csv_w++;
	// 讀入色碼對應表
	std::ifstream mapfile(argv[2]);
	for (;mapfile.good();)
	{
		char line[100] = { 0 };
		mapfile.getline(line, 100);
		char name[100] = { 0 };
		int value;
		sscanf(line, "%s %d", name, &value);
		TextMapping.insert({{name,value}});
	}
	cv::Mat pic;
	pic.create(csv_h, csv_w, CV_32FC1);
	std::ifstream csvfile2(argv[1]);
	// 上色
	int now_x = 0, now_y = 0;
	for (; csvfile2.good() && now_y< csv_h;)
	{
		now_x = 0;
		char line[65536] = { 0 };
		csvfile2.getline(line, 65536);
		char name[100]={0};
		int namepos = 0;
		for (int pos = 0;; ++pos)
		{
			
			if (line[pos] == ',' || line[pos] == 0)
			{
				if (namepos <= 1)
				{
					int v = TextMapping["BG"];
					pic.at<float>(now_y, now_x) = v;
				}
				else
				{
					name[namepos] = 0;
					if (TextMapping.find(name) != TextMapping.end())
					{
						int v = TextMapping[name];
						pic.at<float>(now_y, now_x) = v;
					}
					else
					{
						printf("%s  沒有在色彩對應表\n", name);
						pic.at<float>(now_y, now_x) = 0;
					}
				}
				if (line[pos] == 0)
				{
					break;
				}
				namepos = 0;
				pos++;
				now_x++;
			}
			name[namepos] = line[pos];
			namepos++;
			if (line[pos] == ',')
			{
				pos--;
			}
		}
		now_y++;
	}
	int pic_w = atoi(argv[4]);
	int pic_h = atoi(argv[5]);
	cv::resize(pic.clone(), pic, cv::Size(pic_w, pic_h), 0, 0, cv::INTER_LINEAR);
	int blur1 = atoi(argv[6]);
	int blur2 = atoi(argv[7]);
	int blur3 = atoi(argv[8]);
	blur1 = blur1 % 2 == 0 ? blur1 + 1 : blur1;
	for (int i = 1; i <= blur2; i++)
	{
		//blur(pic.clone(), pic, cv::Size(blur1, blur1));
		bilateralFilter(pic.clone(), pic, blur1, blur3, 0);
	}
	pic.clone().convertTo(pic, CV_16U, 255);
	cv::imshow("pic", pic);
	cv::waitKey();
	cv::imwrite(argv[3], pic);
    return 0;
}

