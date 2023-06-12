#pragma once

#include <map>
#include <vector>
#include <string>
#include <regex>
#include <opencv2/opencv.hpp>

struct TypeInfo
{
	std::string type;
	std::string name;
	uint degree;
	uint count;
	double area;
};

class ColNode
{
public:
	// function
	ColNode(cv::Point3i m = { 0,0,0 }, int id = -1);
	ColNode(std::vector<cv::Point> &c, int d, int id = -1);
	~ColNode();

	void initial(cv::Point3i m = { 0,0,0 }, int id = -1);
	void initial(std::vector<cv::Point> &c, int d, int id = -1);
	void release();

	// data
	int clsId;
	cv::Point3i pos;
	std::vector<cv::Point> contour;
	double area;
};

class ColCluster
{
public:
	// function
	ColCluster(int i, int t);
	~ColCluster();

	void initial(int i, int t);
	void release();

	void addNode(ColNode *node);
	bool findType(int t);
	cv::Point3i getPos();
	double getArea();
	int getHeight();

	// data
	int id;
	int type;
	std::vector<ColNode*> nodeList;
	std::vector<int> typeList;
	void *cls;
};

class LayerData
{
public:
	// function
	LayerData();
	LayerData(std::string n, cv::Vec3b c, cv::Size s, unsigned int d, unsigned char t = 0);
	~LayerData();

	void initial(std::string n, cv::Vec3b c, cv::Size s, unsigned int d, unsigned char t = 0);
	void release();
	void resetCluster();
	void resetType();

	// data
	std::string name;
	cv::Vec3b color;
	cv::Vec3b ori;
	unsigned char display;
	unsigned char type;
	unsigned char ks;
	unsigned char maxVal;
	unsigned char minVal;
	double percent;
	double weight;
	double meanArea;
	double meanHeight;
	cv::Rect roi;
	std::vector<cv::Mat> matList;
	std::vector<std::vector<ColNode>> nodeList;
	std::map<uint, ColCluster> clusterList;
	std::vector<std::vector<cv::Point>> pointList;
};

class ImageData
{
public:
	// function
	ImageData(std::string n = "", cv::Size s = cv::Size(0, 0), unsigned int d = 0);
	~ImageData();

	void initial(std::string n = "", cv::Size s = cv::Size(0, 0), unsigned int d = 0);
	void release();

	void addLayer(std::string n, cv::Vec3b c, unsigned char t = 0);
	void delLayer(unsigned int id);
	void resetLayer(unsigned int id);
	void resetCellLayer();
	cv::Mat getImage(unsigned int d);
	cv::Mat getCell(unsigned int d);
	cv::Vec3b name2col(std::string name);
	void normalize(unsigned int id, unsigned int d);
	void calcMode();
	void calcCluster(int id);
	void calcType(int ref, int mode);
	std::vector<TypeInfo> calcTypeInfo(int ref, cv::Rect roi);

	cv::Mat closeConcaveHull(cv::Mat src, double meanArea = 0.0);
	cv::Mat fillHole(cv::Mat src, double meanArea = 0.0);
	cv::Mat openOperation(cv::Mat src, int ksize = 3);
	cv::Mat closeOperation(cv::Mat src, int ksize = 3);
	cv::Mat edgeOperation(cv::Mat src, int ksize = 3);
	cv::Mat medianFilter(cv::Mat src, int ksize = 3);
	cv::Mat binaryzation(cv::Mat src, int thr = -1);
	double calcMeanArea(cv::Mat src, int ksize = 3);

	// data
	std::string fileName;
	cv::Size size;
	unsigned int depth;
	unsigned int nucId;
	unsigned int imgMode;
	std::vector<LayerData> layerList;
	LayerData cellLayer;
};
