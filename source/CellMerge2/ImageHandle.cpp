#include "ImageHandle.h"

using namespace std;
using namespace cv;

ColNode::ColNode(Point3i m, int id)
{
	initial(m, id);
}

ColNode::ColNode(vector<Point> &c, int d, int id)
{
	initial(c, d, id);
}

ColNode::~ColNode()
{
	release();
}

void ColNode::initial(Point3i m, int id)
{
	release();
	clsId = id;
	pos = m;
	area = 0.0;
}

void ColNode::initial(vector<Point> &c, int d, int id)
{
	release();
	clsId = id;
	if (c.size() > 0)
	{
		Moments m = moments(c);
		pos = { (int)round(m.m10 / m.m00), (int)round(m.m01 / m.m00), d };
		for (auto term : c) contour.push_back(term);
		area = fabs(contourArea(contour));
	}
	else
	{
		pos = { 0, 0, 0 };
		area = 0.0;
	}
}

void ColNode::release()
{
	vector<Point>().swap(contour);
	clsId = -1;
}

ColCluster::ColCluster(int i, int t)
{
	initial(i, t);
}

ColCluster::~ColCluster()
{
	release();
}

void ColCluster::initial(int i, int t)
{
	release();
	id = i;
	type = t;
	typeList.push_back(t);
}

void ColCluster::release()
{
	vector<ColNode*>().swap(nodeList);
	vector<int>().swap(typeList);
}

void ColCluster::addNode(ColNode *node)
{
	node->clsId = id;
	nodeList.push_back(node);
}

Point3i ColCluster::getPos()
{
	Point3i pos = Point3i(0, 0, 0);
	if (nodeList.size() == 0) return pos;
	for (size_t i = 0; i < nodeList.size(); i++) pos += nodeList[i]->pos;
	pos.x = pos.x / nodeList.size();
	pos.y = pos.y / nodeList.size();
	pos.z = pos.z / nodeList.size();
	return pos;
}

double ColCluster::getArea()
{
	double area = 0.0;
	for (size_t i = 0; i < nodeList.size(); i++) area = max(area, nodeList[i]->area);
	return area;
}

int ColCluster::getHeight()
{
	if (nodeList.size() == 0) return 0;
	int depthMin = nodeList[0]->pos.z;
	int depthMax = nodeList[0]->pos.z;
	for (size_t i = 1; i < nodeList.size(); i++)
	{
		depthMin = min(depthMin, nodeList[i]->pos.z);
		depthMax = max(depthMax, nodeList[i]->pos.z);
	}
	return depthMax - depthMin;
}

bool ColCluster::findType(int t)
{
	for (auto term : typeList) if (term == t) return true;
	return false;
}

LayerData::LayerData()
{
	initial("", Vec3b(0, 0, 0), Size(0, 0), 0, 0);
}

LayerData::LayerData(string n, Vec3b c, Size s, unsigned int d, unsigned char t)
{
	initial(n, c, s, d, t);
}

LayerData::~LayerData()
{
	release();
}

void LayerData::initial(string n, Vec3b c, Size s, unsigned int d, unsigned char t)
{
	release();
	name = n;
	color = c;
	ori = c;
	type = t;
	ks = 2;
	maxVal = 255;
	minVal = 1;
	percent = 1.0;
	display = 1;
	weight = 1.0;
	meanArea = 0.0;
	meanHeight = 0.0;
	if (t == 0) for (int i = 0; i < d; i++) matList.push_back(Mat::zeros(s, CV_8UC1));
}

void LayerData::release()
{
	resetCluster();
	for (size_t i = 0; i < matList.size(); i++) matList[i].release();
	vector<Mat>().swap(matList);
}

void LayerData::resetCluster()
{
	for (map<uint, ColCluster>::iterator i = clusterList.begin(); i != clusterList.end(); i++) i->second.release();
	map<uint, ColCluster>().swap(clusterList);
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		for (size_t j = 0; j < nodeList[i].size(); j++) nodeList[i][j].release();
		vector<ColNode>().swap(nodeList[i]);
	}
	vector<vector<ColNode>>().swap(nodeList);
	for (size_t i = 0; i < pointList.size(); i++) vector<Point>().swap(pointList[i]);
	vector<vector<Point>>().swap(pointList);
}

void LayerData::resetType()
{
	for (map<uint, ColCluster>::iterator i = clusterList.begin(); i != clusterList.end(); i++)
	{
		vector<int>().swap(i->second.typeList);
		i->second.typeList.push_back(i->second.type);
	}
}

ImageData::ImageData(string n, Size s, unsigned int d)
{
	initial(n, s, d);
}

ImageData::~ImageData()
{
	release();
}

void ImageData::initial(string n, Size s, unsigned int d)
{
	release();
	fileName = n;
	size = s;
	depth = d;
	nucId = 0;
	imgMode = 0;
	cellLayer.initial("Cell", Vec3b(255, 255, 255), s, 0, 1);
}

void ImageData::release()
{
	cellLayer.release();
	for (size_t i = 0; i < layerList.size(); i++) layerList[i].release();
	vector<LayerData>().swap(layerList);
}

void ImageData::calcMode()
{
	vector<int> cts;
	if (size.area() == 0 || depth == 0) return;
	Mat res = Mat::zeros(size, CV_8UC3);
	Vec3b pxs[256];
	for (size_t i = 0; i < layerList.size(); i++)
	{
		for (int k = 0; k < 256; k++) pxs[k] = layerList[i].color * (k / 255.0);
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
		{
			Vec3b pixel = pxs[layerList[i].matList[0].at<uchar>(x, y)];
			res.at<Vec3b>(x, y)[0] = max(res.at<Vec3b>(x, y)[0], pixel[0]);
			res.at<Vec3b>(x, y)[1] = max(res.at<Vec3b>(x, y)[1], pixel[1]);
			res.at<Vec3b>(x, y)[2] = max(res.at<Vec3b>(x, y)[2], pixel[2]);
		}
	}
	Mat img;
	cvtColor(res, img, COLOR_RGB2GRAY);
	vector<int> valCount(16, 0);
	for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++) valCount[img.at<uchar>(x, y) / 16]++;
	int fid = 0, sid = 15;
	for (int i = 0; i < 16; i++) if (valCount[i] >= valCount[fid]) fid = i;
	for (int i = 15; i >= 0; i--) if (valCount[i] >= valCount[sid] && i != fid) sid = i;
	if (abs(fid - sid) > 2) imgMode = min(fid, sid) < 4 ? 0 : 1; else imgMode = fid < 4 ? 0 : 1;
	img.release();
	res.release();
}

void ImageData::addLayer(string n, Vec3b c, unsigned char t)
{
	layerList.push_back(LayerData(n, c, size, depth, t));
}

void ImageData::delLayer(unsigned int id)
{
	if (id >= layerList.size() || layerList[id].type == 0) return;
	layerList[id].release();
	layerList.erase(layerList.begin() + id);
}

void ImageData::resetLayer(unsigned int id)
{
	if (id >= layerList.size()) return;
	layerList[id].maxVal = 255;
	layerList[id].minVal = 1;
	layerList[id].percent = 1.0;
	layerList[id].weight = 1.0;
	layerList[id].color = layerList[id].ori;
	layerList[id].display = 1;
}

void ImageData::resetCellLayer()
{
	if (nucId >= layerList.size()) return;
	layerList[nucId].maxVal = 255;
	layerList[nucId].minVal = 1;
	layerList[nucId].percent = 1.0;
	layerList[nucId].ks = 2;
	cellLayer.color = cellLayer.ori;
	cellLayer.display = 1;
}

Mat ImageData::getImage(unsigned int d)
{
	Mat res = Mat::zeros(size, CV_8UC3);
	if (d >= depth) return res;
	double coef[256];
	if (imgMode == 1)
	{
		for (int k = 0; k < 256; k++) coef[k] = 255.0;
		if (layerList[0].display != 0)
		{
			uchar maxVal = min((uchar)255, layerList[0].maxVal);
			uchar minVal = max((uchar)1, layerList[0].minVal);
			uchar grayRange = maxVal - minVal + 1;
			double percent = layerList[0].percent;
			double weight = layerList[0].weight;
			coef[0] = min(coef[0], minVal / weight);
			for (int k = 1; k < minVal; k++) coef[k] = min(coef[k], minVal / (k * weight));
			for (int k = minVal; k < maxVal; k++) coef[k] = min(coef[k], ((255.0 - minVal) * pow((1.0 + k - minVal) / grayRange, 1.0 / percent) + minVal) / (k * weight));
			for (int k = maxVal; k < 256; k++) coef[k] = min(coef[k], 255.0 / (k * weight));
		}
		if (layerList[1].display != 0)
		{
			uchar maxVal = min((uchar)255, layerList[1].maxVal);
			uchar minVal = max((uchar)1, layerList[1].minVal);
			uchar grayRange = maxVal - minVal + 1;
			double percent = layerList[1].percent;
			double weight = layerList[1].weight;
			for (int k = 0; k < minVal; k++) coef[k] = min(coef[k], 0.0);
			for (int k = minVal; k < maxVal; k++) coef[k] = min(coef[k], maxVal * pow((1.0 + k - minVal) / grayRange, 1.0 / percent) / (k * weight));
			for (int k = maxVal; k < 256; k++) coef[k] = min(coef[k], 255.0 / (k * weight));
		}
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
		{
			res.at<Vec3b>(x, y)[0] = layerList[2].matList[d].at<uchar>(x, y);
			res.at<Vec3b>(x, y)[1] = layerList[1].matList[d].at<uchar>(x, y);
			res.at<Vec3b>(x, y)[2] = layerList[0].matList[d].at<uchar>(x, y);
		}
		Mat temp;
		cvtColor(res, temp, CV_RGB2GRAY);
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
		{
			double t = coef[temp.at<uchar>(x, y)];
			if (temp.at<uchar>(x, y) == 0) res.at<Vec3b>(x, y) += Vec3b(1, 1, 1);
			res.at<Vec3b>(x, y)[0] = (uchar)min(255.0, round(res.at<Vec3b>(x, y)[0] * t));
			res.at<Vec3b>(x, y)[1] = (uchar)min(255.0, round(res.at<Vec3b>(x, y)[1] * t));
			res.at<Vec3b>(x, y)[2] = (uchar)min(255.0, round(res.at<Vec3b>(x, y)[2] * t));
		}
		temp.release();
	}
	else for (size_t i = 0; i < layerList.size(); i++)
	{
		if (layerList[i].type > 0 || layerList[i].display == 0) continue;
		Vec3b pxs[256];
		uchar maxVal = min((uchar)255, layerList[i].maxVal);
		uchar minVal = max((uchar)1, layerList[i].minVal);
		uchar grayRange = maxVal - minVal + 1;
		double percent = layerList[i].percent;
		double weight = layerList[i].weight;
		for (int k = 0; k < minVal; k++) coef[k] = 0.0;
		for (int k = minVal; k < maxVal; k++) coef[k] = weight * pow((1.0 + k - minVal) / grayRange, 1.0 / percent);
		for (int k = maxVal; k < 256; k++) coef[k] = weight * 255.0;
		for (int k = 0; k < 256; k++) pxs[k] = layerList[i].color * coef[k];
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
		{
			Vec3b pixel = pxs[layerList[i].matList[d].at<uchar>(x, y)];
			res.at<Vec3b>(x, y)[0] = max(res.at<Vec3b>(x, y)[0], pixel[0]);
			res.at<Vec3b>(x, y)[1] = max(res.at<Vec3b>(x, y)[1], pixel[1]);
			res.at<Vec3b>(x, y)[2] = max(res.at<Vec3b>(x, y)[2], pixel[2]);
		}
	}
	return res;
}

Mat ImageData::getCell(unsigned int d)
{
	Mat res = getImage(d);
	if (d >= depth || cellLayer.display == 0) return res;
	int ksize = layerList[nucId].ks;
	double coef[256];
	Mat nuc = Mat::zeros(size, CV_8UC1);
	if (imgMode == 1)
	{
		uchar maxVal = min((uchar)255, layerList[nucId].maxVal);
		uchar minVal = max((uchar)1, layerList[nucId].minVal);
		uchar grayRange = maxVal - minVal + 1;
		double percent = layerList[nucId].percent;
		for (int k = 0; k < 256; k++) coef[k] = 255.0;
		if (nucId == 0)
		{
			for (int k = 0; k < minVal; k++) coef[k] = minVal;
			for (int k = minVal; k < maxVal; k++) coef[k] = (255.0 - minVal) * pow((1.0 + k - minVal) / grayRange, 1.0 / percent) + minVal;
		}
		if (nucId == 1)
		{
			for (int k = 0; k < minVal; k++) coef[k] = 0.0;
			for (int k = minVal; k < maxVal; k++) coef[k] = maxVal * pow((1.0 + k - minVal) / grayRange, 1.0 / percent);
		}
		for (int k = 0; k < 256; k++) coef[k] = 255.0 - coef[k];
		Mat temp = Mat::zeros(size, CV_8UC3);
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
		{
			temp.at<Vec3b>(x, y)[0] = layerList[2].matList[d].at<uchar>(x, y);
			temp.at<Vec3b>(x, y)[1] = layerList[1].matList[d].at<uchar>(x, y);
			temp.at<Vec3b>(x, y)[2] = layerList[0].matList[d].at<uchar>(x, y);
		}
		cvtColor(temp, nuc, CV_RGB2GRAY);
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++) nuc.at<uchar>(x, y) = (uchar)coef[nuc.at<uchar>(x, y)];
		temp.release();
	}
	else
	{
		uchar maxVal = min((uchar)255, layerList[nucId].maxVal);
		uchar minVal = max((uchar)1, layerList[nucId].minVal);
		uchar grayRange = maxVal - minVal + 1;
		double percent = layerList[nucId].percent;
		for (int k = 0; k < minVal; k++) coef[k] = 0.0;
		for (int k = minVal; k < maxVal; k++) coef[k] = round(255.0 * pow((1.0 + k - minVal) / grayRange, 1.0 / percent));
		for (int k = maxVal; k < 256; k++) coef[k] = 255.0;
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++) nuc.at<uchar>(x, y) = (uchar)coef[layerList[nucId].matList[d].at<uchar>(x, y)];
	}
	nuc = closeOperation(nuc, ksize);
	nuc = openOperation(nuc, ksize);
	nuc = binaryzation(nuc);
	nuc = openOperation(nuc, ksize);
	double ma = calcMeanArea(nuc, ksize);
	if (ma > ksize) nuc = closeConcaveHull(nuc, ma * 2);
	nuc = openOperation(nuc, ksize);
	ma = nuc.at<uchar>(0, 0);
	for (int x = 0; x < size.height; x++)
	{
		for (int y = 0; y < size.width; y++) if (nuc.at<uchar>(x, y) != ma)
		{
			ma = nuc.at<uchar>(x, y);
			break;
		}
		if (nuc.at<uchar>(0, 0) != ma) break;
	}
	if (nuc.at<uchar>(0, 0) == ma)
	{
		nuc.release();
		return res;
	}
	ma = calcMeanArea(nuc, ksize);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(nuc, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	if (contours.size() == 0) return res;
	int thk = max(1, max(size.width, size.height) / 1000);
	for (size_t i = 0; i < contours.size(); i++) if (hierarchy[i][3] == -1)
	{
		double area = fabs(contourArea(contours[i]));
		if (hierarchy[i][2] != -1) area -= fabs(contourArea(contours[hierarchy[i][2]]));
		if (area >= ma / 4 && area <= ma * 4) drawContours(res, contours, i, cellLayer.color, thk);
		else if (area >= ksize) drawContours(res, contours, i, cellLayer.color / 2, thk);
	}
	//if (d < layerList[nucId].pointList.size()) for (auto pos : layerList[nucId].pointList[d]) circle(res, pos, 5, cellLayer.color, -1);
	nuc.release();
	return res;
}

Vec3b ImageData::name2col(string name)
{
	Vec3b col = { 0, 0, 0 };
	if (name == "Cyan") col = { 0, 255, 255 };
	else if (name == "Magenta") col = { 255, 0, 255 };
	else if (name == "Yellow") col = { 255, 255, 0 };
	else if (name == "Gray") col = { 160, 160, 160 };
	else if (name == "Red") col = { 255, 0, 0 };
	else if (name == "Green") col = { 0, 255, 0 };
	else if (name == "Blue") col = { 0, 0, 255 };
	else if (name == "Black") col = { 0, 0, 0 };
	else if (name == "White") col = { 255, 255, 255 };
	return col;
}

void ImageData::normalize(unsigned int id, unsigned int d)
{
	if (id >= layerList.size() || d >= depth || layerList[id].type > 0) return;
	uchar grayMax = 0;
	uchar graySMax = 0;
	uchar grayMin = 255;
	for (int i = 0; i < size.height; i++) for (int j = 0; j < size.width; j++)
	{
		if (graySMax < layerList[id].matList[d].at<uchar>(i, j) && layerList[id].matList[d].at<uchar>(i, j) < 254) graySMax = layerList[id].matList[d].at<uchar>(i, j);
		if (grayMax < layerList[id].matList[d].at<uchar>(i, j)) grayMax = layerList[id].matList[d].at<uchar>(i, j);
		if (grayMin > layerList[id].matList[d].at<uchar>(i, j)) grayMin = layerList[id].matList[d].at<uchar>(i, j);
	}
	if (grayMax - graySMax > 20) grayMax = graySMax;
	if (grayMin == 0) grayMin = 1;
	int grayRange = grayMax - grayMin + 1;
	if (grayRange < 5) return;
	double coef[256];
	for (int i = 0; i < grayMin; i++) coef[i] = 0.0;
	for (int i = grayMin; i < grayMax; i++) coef[i] = round(255.0 * (i - grayMin + 1) / grayRange);
	for (int i = grayMax; i < 256; i++) coef[i] = 255.0;
	for (int i = 0; i < size.height; i++) for (int j = 0; j < size.width; j++)
	{
		layerList[id].matList[d].at<uchar>(i, j) = (uchar)coef[layerList[id].matList[d].at<uchar>(i, j)];
	}
}

Mat ImageData::closeConcaveHull(Mat src, double meanArea)
{
	Mat dst = src.clone();
	vector<vector<Point>> contours;
	findContours(dst, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	if (contours.size() == 0) return dst;
	vector<vector<Point>> hulls(contours.size());
	for (uint i = 0; i < contours.size(); i++) convexHull(Mat(contours[i]), hulls[i]);
	Mat imgDiff;
	Mat imgContours = Mat::zeros(dst.size(), CV_8UC1);
	Mat imgHulls = Mat::zeros(dst.size(), CV_8UC1);
	for (uint i = 0; i < contours.size(); i++)
	{
		drawContours(imgContours, contours, i, Scalar(255), -1);
		drawContours(imgHulls, hulls, i, Scalar(255), -1);
	}
	absdiff(imgHulls, imgContours, imgDiff);
	vector<vector<Point>> diffs(contours.size());
	findContours(imgDiff, diffs, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	for (uint i = 0; i < diffs.size(); i++) if (fabs(contourArea(diffs[i])) < meanArea || meanArea == 0.0) drawContours(dst, diffs, i, Scalar(255), -1);
	return dst;
}

Mat ImageData::fillHole(Mat src, double meanArea)
{
	Mat dst = src.clone();
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dst, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	for (uint i = 0; i < contours.size(); i++)
	{
		if (hierarchy[i][3] != -1 && (fabs(contourArea(contours[i])) < meanArea || meanArea == 0.0)) drawContours(dst, contours, i, Scalar(255), -1);
	}
	return dst;
}

Mat ImageData::openOperation(Mat src, int ksize)
{
	if (ksize < 2) return src.clone();
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(ksize, ksize), Point(ksize / 2, ksize / 2));
	Mat dst, temp;
	erode(src, temp, element);
	dilate(temp, dst, element);
	return dst;
}

Mat ImageData::closeOperation(Mat src, int ksize)
{
	if (ksize < 2) return src.clone();
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(ksize, ksize), Point(ksize / 2, ksize / 2));
	Mat dst, temp;
	dilate(src, temp, element);
	erode(temp, dst, element);
	return dst;
}

Mat ImageData::edgeOperation(Mat src, int ksize)
{
	Mat dst = src.clone();
	Mat grad_x, grad_y;
	Sobel(dst, grad_x, CV_16S, 1, 0);
	convertScaleAbs(grad_x, grad_x);
	Sobel(dst, grad_y, CV_16S, 0, 1);
	convertScaleAbs(grad_y, grad_y);
	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dst);
	return dst;
}

Mat ImageData::medianFilter(Mat src, int ksize)
{
	if (ksize < 2) return src.clone();
	Mat dst;
	medianBlur(src, dst, ksize);
	return dst;
}

Mat ImageData::binaryzation(Mat src, int thr)
{
	Mat dst = src.clone();
	if (thr == -1)
	{
		Mat temp = src.clone();
		uchar t = 255;
		for (int i = 0; i < src.size().height; i++) for (int j = 0; j < src.size().width; j++) if (src.at<uchar>(i, j) > 0) t = min(t, src.at<uchar>(i, j));
		threshold(temp, dst, t, 255, CV_THRESH_OTSU);
	}
	else for (int i = 0; i < src.size().height; i++) for (int j = 0; j < src.size().width; j++)
	{
		dst.at<uchar>(i, j) = src.at<uchar>(i, j) < thr ? 0 : 255;
	}
	return dst;
}

double ImageData::calcMeanArea(Mat src, int ksize)
{
	Mat dst = src.clone();
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dst, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	if (contours.size() == 0) return ksize;
	vector<double> area;
	for (uint i = 0; i < contours.size(); i++) if (hierarchy[i][3] == -1)
	{
		double temp = fabs(contourArea(contours[i]));
		if (hierarchy[i][2] != -1) temp -= fabs(contourArea(contours[hierarchy[i][2]]));
		if (temp > ksize) area.push_back(temp);
	}
	if (area.size() == 0) return ksize;
	std::sort(area.begin(), area.end());
	uint bid = area.size() * 50 / 100;
	uint eid = area.size() * 90 / 100;
	if (eid == 0) eid = 1;
	if (bid == eid) bid = eid - 1;
	double meanArea = 0.0;
	for (uint i = bid; i < eid; i++) meanArea += area[i];
	meanArea = meanArea / (eid - bid);
	return meanArea;
}

void ImageData::calcCluster(int id)
{
	if (size.area() == 0 || id >= layerList.size() || (imgMode == 1 && id > 1)) return;
	uint ksize = layerList[id].ks;
	layerList[id].resetCluster();
	vector<vector<ColNode>>(depth).swap(layerList[id].nodeList);
	vector<vector<Point>>(depth).swap(layerList[id].pointList);

	vector<double> areaList;
	double coef[256];
	Mat nuc = Mat::zeros(size, CV_8UC1);
	for (int d = 0; d < depth; d++)
	{
		if (imgMode == 1)
		{
			uchar maxVal = min((uchar)255, layerList[id].maxVal);
			uchar minVal = max((uchar)1, layerList[id].minVal);
			uchar grayRange = maxVal - minVal + 1;
			double percent = layerList[id].percent;
			for (int k = 0; k < 256; k++) coef[k] = 255.0;
			if (id == 0)
			{
				for (int k = 0; k < minVal; k++) coef[k] = minVal;
				for (int k = minVal; k < maxVal; k++) coef[k] = (255.0 - minVal) * pow((1.0 + k - minVal) / grayRange, 1.0 / percent) + minVal;
			}
			if (id == 1)
			{
				for (int k = 0; k < minVal; k++) coef[k] = 0.0;
				for (int k = minVal; k < maxVal; k++) coef[k] = maxVal * pow((1.0 + k - minVal) / grayRange, 1.0 / percent);
			}
			for (int k = 0; k < 256; k++) coef[k] = 255.0 - coef[k];
			Mat temp = Mat::zeros(size, CV_8UC3);
			for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
			{
				temp.at<Vec3b>(x, y)[0] = layerList[2].matList[d].at<uchar>(x, y);
				temp.at<Vec3b>(x, y)[1] = layerList[1].matList[d].at<uchar>(x, y);
				temp.at<Vec3b>(x, y)[2] = layerList[0].matList[d].at<uchar>(x, y);
			}
			cvtColor(temp, nuc, CV_RGB2GRAY);
			for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++) nuc.at<uchar>(x, y) = (uchar)coef[nuc.at<uchar>(x, y)];
			temp.release();
		}
		else
		{
			uchar maxVal = min((uchar)255, layerList[id].maxVal);
			uchar minVal = max((uchar)1, layerList[id].minVal);
			uchar grayRange = maxVal - minVal + 1;
			double percent = layerList[id].percent;
			for (int k = 0; k < minVal; k++) coef[k] = 0.0;
			for (int k = minVal; k < maxVal; k++) coef[k] = round(255.0 * pow((1.0 + k - minVal) / grayRange, 1.0 / percent));
			for (int k = maxVal; k < 256; k++) coef[k] = 255.0;
			for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++) nuc.at<uchar>(x, y) = (uchar)coef[layerList[id].matList[d].at<uchar>(x, y)];
		}
		nuc = closeOperation(nuc, ksize);
		nuc = openOperation(nuc, ksize);
		nuc = binaryzation(nuc);
		nuc = openOperation(nuc, ksize);
		double ma = calcMeanArea(nuc, ksize);
		if (ma > (double)ksize) nuc = closeConcaveHull(nuc, ma * 2);
		nuc = openOperation(nuc, ksize);
		ma = nuc.at<uchar>(0, 0);
		for (int x = 0; x < size.height; x++)
		{
			for (int y = 0; y < size.width; y++) if (nuc.at<uchar>(x, y) != ma)
			{
				ma = nuc.at<uchar>(x, y);
				break;
			}
			if (nuc.at<uchar>(0, 0) != ma) break;
		}
		if (nuc.at<uchar>(0, 0) == ma) continue;
		ma = calcMeanArea(nuc, ksize);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(nuc, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
		for (size_t i = 0; i < contours.size(); i++) if (hierarchy[i][3] == -1)
		{
			double area = fabs(contourArea(contours[i]));
			if (hierarchy[i][2] != -1) area -= fabs(contourArea(contours[hierarchy[i][2]]));
			if (area >= ksize) layerList[id].nodeList[d].push_back(ColNode(contours[i], d));
			if (area >= ma / 4 && area <= ma * 4) areaList.push_back(area);
		}
	}
	layerList[id].meanArea = 0.0;
	if (areaList.size() > 0)
	{
		for (auto area : areaList) layerList[id].meanArea += area;
		layerList[id].meanArea = max((double)layerList[id].ks, layerList[id].meanArea / areaList.size());
	}
	nuc.release();

	Mat da, db;
	uint clsCount = 1;
	if (depth < 2) for (size_t i = 0; i < layerList[id].nodeList[0].size(); i++)
	{
		layerList[id].clusterList.insert(pair<uint, ColCluster>(clsCount, ColCluster(clsCount, id)));
		layerList[id].clusterList.at(clsCount).addNode(&layerList[id].nodeList[0][i]);
		clsCount++;
	}
	else for (size_t d = 0; d < depth - 1; d++)
	{
		if (layerList[id].nodeList[d].size() == 0) continue;
		da = Mat::zeros(size, CV_32SC1);
		db = Mat::zeros(size, CV_32SC1);
		for (size_t i = 0; i < layerList[id].nodeList[d].size(); i++) drawContours(da, vector<vector<Point>>(1, layerList[id].nodeList[d][i].contour), 0, Scalar(i + 1), -1);
		for (size_t i = 0; i < layerList[id].nodeList[d + 1].size(); i++) drawContours(db, vector<vector<Point>>(1, layerList[id].nodeList[d + 1][i].contour), 0, Scalar(i + 1), -1);
		vector<map<uint, uint>> countList(layerList[id].nodeList[d].size());
		for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
		{
			if (da.at<uint>(x, y) == 0 || db.at<uint>(x, y) == 0) continue;
			countList[da.at<uint>(x, y) - 1][db.at<uint>(x, y) - 1] += 1;
		}
		for (size_t i = 0; i < layerList[id].nodeList[d].size(); i++)
		{
			uint aa = (uint)fabs(contourArea(layerList[id].nodeList[d][i].contour));
			if (aa < ksize) continue;
			if (layerList[id].nodeList[d][i].clsId == -1)
			{
				layerList[id].clusterList.insert(pair<uint, ColCluster>(clsCount, ColCluster(clsCount, id)));
				layerList[id].clusterList.at(clsCount).addNode(&layerList[id].nodeList[d][i]);
				clsCount++;
			}
			for (auto term : countList[i])
			{
				if (term.second < max(ksize, max(aa, (uint)fabs(contourArea(layerList[id].nodeList[d + 1][term.first].contour))) / 2)) continue;
				if (layerList[id].nodeList[d + 1][term.first].clsId == layerList[id].nodeList[d][i].clsId) continue;
				if (layerList[id].nodeList[d + 1][term.first].clsId == -1)
				{
					layerList[id].clusterList.at(layerList[id].nodeList[d][i].clsId).addNode(&layerList[id].nodeList[d + 1][term.first]);
				}
				else
				{
					uint oriId = layerList[id].nodeList[d][i].clsId;
					ColCluster *clsSrc = &layerList[id].clusterList.at(layerList[id].nodeList[d][i].clsId);
					ColCluster *clsDst = &layerList[id].clusterList.at(layerList[id].nodeList[d + 1][term.first].clsId);
					for (size_t j = 0; j < clsSrc->nodeList.size(); j++) clsDst->addNode(clsSrc->nodeList[j]);
					clsSrc->release();
					layerList[id].clusterList.erase(oriId);
				}
			}
		}
	}
	da.release();
	db.release();
	
	vector<double> heightList;
	for (map<uint, ColCluster>::iterator i = layerList[id].clusterList.begin(); i != layerList[id].clusterList.end(); i++)
	{
		Point3i pos = i->second.getPos();
		layerList[id].pointList[pos.z].push_back(Point(pos.x, pos.y));
		heightList.push_back(i->second.getHeight());
	}
	layerList[id].meanHeight = 0.0;
	if (heightList.size() > 0)
	{
		for (auto height : heightList) layerList[id].meanHeight += height;
		layerList[id].meanHeight = max(1.0, layerList[id].meanHeight / heightList.size());
	}
}

void ImageData::calcType(int ref, int mode)
{
	if (layerList[ref].nodeList.size() == 0 || mode > 4) return;
	if (mode == 0)
	{
		Mat da, db;
		map<uint, map<uint, uint>> countList;
		for (size_t i = 0; i < layerList.size(); i++) layerList[i].resetType();
		for (map<uint, ColCluster>::iterator i = layerList[ref].clusterList.begin(); i != layerList[ref].clusterList.end(); i++)
		{
			countList.insert(pair<uint, map<uint, uint>>(i->second.id, map<uint, uint>()));
		}
		for (size_t d = 0; d < depth; d++)
		{
			if (layerList[ref].nodeList[d].size() == 0) continue;
			da = Mat::zeros(size, CV_32SC1);
			for (size_t i = 0; i < layerList[ref].nodeList[d].size(); i++)
			{
				drawContours(da, vector<vector<Point>>(1, layerList[ref].nodeList[d][i].contour), 0, Scalar(layerList[ref].nodeList[d][i].clsId + 1), -1);
			}
			for (size_t id = 0; id < layerList.size(); id++)
			{
				if (id == ref || layerList[id].nodeList.size() == 0 || layerList[id].nodeList[d].size() == 0) continue;
				db = Mat::zeros(size, CV_32SC1);
				for (size_t i = 0; i < layerList[id].nodeList[d].size(); i++)
				{
					drawContours(db, vector<vector<Point>>(1, layerList[id].nodeList[d][i].contour), 0, Scalar(id + 1), -1);
				}
				for (int x = 0; x < size.height; x++) for (int y = 0; y < size.width; y++)
				{
					if (da.at<uint>(x, y) == 0 || db.at<uint>(x, y) == 0) continue;
					countList.at(da.at<uint>(x, y) - 1)[db.at<uint>(x, y) - 1] += 1;
				}
			}
		}
		for (map<uint, map<uint, uint>>::iterator i = countList.begin(); i != countList.end(); i++)
		{
			for (map<uint, uint>::iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				layerList[ref].clusterList.at(i->first).typeList.push_back(j->first);
			}
		}
		da.release();
		db.release();
	}
	else
	{
		Point3d pa, pb;
		double dstLim = sqrt(layerList[ref].meanArea) * 2 / 3;
		double hiRate = sqrt(layerList[ref].meanArea) / max(1.0, min(layerList[ref].meanHeight, depth / 2.0));
		double scale = 1.0;
		if (mode == 2) scale = 2.0;
		if (mode == 3) scale = 0.5;
		if (mode == 4) scale = 0.25;
		hiRate = pow(hiRate * scale, 2);
		dstLim = pow(max((double)layerList[ref].ks, dstLim * scale), 2);
		for (size_t i = 0; i < layerList.size(); i++) layerList[i].resetType();
		for (map<uint, ColCluster>::iterator i = layerList[ref].clusterList.begin(); i != layerList[ref].clusterList.end(); i++)
		{
			pa = i->second.getPos();
			for (size_t id = 0; id < layerList.size(); id++)
			{
				if (id == ref || layerList[id].clusterList.size() == 0) continue;
				for (map<uint, ColCluster>::iterator j = layerList[id].clusterList.begin(); j != layerList[id].clusterList.end(); j++)
				{
					pb = pa - (Point3d)j->second.getPos();
					if (pow(pb.x, 2) + pow(pb.y, 2) + pow(pb.z, 2) < dstLim)
					{
						i->second.typeList.push_back(id);
						break;
					}
				}
			}
		}
	}
}

vector<TypeInfo> ImageData::calcTypeInfo(int ref, Rect roi)
{
	if (size.area() == 0 || roi.area() == 0) return vector<TypeInfo>();
	if (layerList[ref].clusterList.size() == 0)
	{
		vector<TypeInfo> infoList(1);
		infoList[0].type = "-";
		infoList[0].name = "-";
		infoList[0].degree = 0;
		infoList[0].area = 0;
		infoList[0].count = 0;
		return infoList;
	}

	vector<double> meanArea(layerList.size());
	for (size_t id = 0; id < layerList.size(); id++)
	{
		vector<double> areaList;
		for (map<uint, ColCluster>::iterator it = layerList[ref].clusterList.begin(); it != layerList[ref].clusterList.end(); it++)
		{
			double area = it->second.getArea();
			uint height = it->second.getHeight();
			Point3i pos = it->second.getPos();
			if (area < layerList[ref].meanArea / 4 || area > layerList[ref].meanArea * 4 || height > depth - 1) continue;
			if (pos.x < roi.x || pos.x > roi.x + roi.width || pos.y < roi.y || pos.y > roi.y + roi.height) continue;
			areaList.push_back(area);
		}
		meanArea[id] = 0.0;
		if (areaList.size() > 0)
		{
			for (auto t : areaList) meanArea[id] += t;
			meanArea[id] = meanArea[id] / areaList.size();
		}
	}

	map<string, uint> typeCount;
	for (map<uint, ColCluster>::iterator it = layerList[ref].clusterList.begin(); it != layerList[ref].clusterList.end(); it++)
	{
		double area = it->second.getArea();
		uint height = it->second.getHeight();
		Point3i pos = it->second.getPos();
		if (area < layerList[ref].meanArea / 4 || area > layerList[ref].meanArea * 4 || height > depth - 1) continue;
		if (pos.x < roi.x || pos.x > roi.x + roi.width || pos.y < roi.y || pos.y > roi.y + roi.height) continue;
		vector<string> typeList;
		for (int i = 0; i < layerList.size(); i++)
		{
			if (i == ref) continue;
			if (it->second.findType(i)) typeList.push_back(to_string(i));
		}
		string type = typeList.size() == 0 ? "" : typeList[0];
		for (size_t i = 1; i < typeList.size(); i++) type += ";" + typeList[i];
		typeCount[type] += 1;
	}

	int count = 0;
	regex re(";");
	vector<TypeInfo> infoList(typeCount.size());
	for (map<string, uint>::iterator it = typeCount.begin(); it != typeCount.end(); it++)
	{
		if (it->first == "")
		{
			infoList[count].type = "-";
			infoList[count].name = "-";
			infoList[count].degree = 0;
			infoList[count].area = layerList[ref].meanArea;
		}
		else
		{
			vector<string> typeList = { sregex_token_iterator(it->first.begin(), it->first.end(), re, -1), sregex_token_iterator() };
			infoList[count].type = it->first;
			infoList[count].name = typeList.size() == 0 ? "" : layerList[atoi(typeList[0].c_str())].name;
			for (size_t i = 1; i < typeList.size(); i++) infoList[count].name += ";" + layerList[atoi(typeList[i].c_str())].name;
			infoList[count].degree = typeList.size();
			infoList[count].area = infoList[count].degree == 1 ? layerList[atoi(typeList[0].c_str())].meanArea : 0.0;
		}
		infoList[count].count = it->second;
		count++;
	}
	return infoList;
}
