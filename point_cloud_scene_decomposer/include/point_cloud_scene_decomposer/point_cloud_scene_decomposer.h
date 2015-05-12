// Copyright (C) 2015 by Krishneel Chaudhary

#ifndef _POINT_CLOUD_SCENE_DECOMPOSER_H_ 
#define _POINT_CLOUD_SCENE_DECOMPOSER_H_

#include <point_cloud_scene_decomposer/constants.h>
#include <point_cloud_scene_decomposer/scene_decomposer_image_processor.h>
#include <point_cloud_scene_decomposer/region_adjacency_graph.h>

class PointCloudSceneDecomposer: public SceneDecomposerImageProcessor {
 private:
    ros::NodeHandle nh_;
    ros::Publisher pub_image_;
    ros::Publisher pub_cloud_;
    ros::Subscriber sub_cloud_;
    ros::Subscriber sub_image_;

    pcl::PointCloud<PointT>::Ptr pcl_cloud__;
    pcl::PointCloud<PointT>::Ptr filter_cloud__;

    //! Variable for distance filter
    const float MAX_DISTANCE;
    const float MIN_DISTANCE;

    void pclNearestNeigborSearch(
        pcl::PointCloud<pcl::PointXYZ>::Ptr, std::vector<std::vector<int> > &,
        bool isneigbour = true, const int = 8, const double = 0.05);

    void pointCloud2RGBDImage(
        pcl::PointCloud<PointT>::Ptr, cv::Mat &, cv::Mat &);

    void semanticCloudLabel(
        const std::vector<pcl::PointCloud<PointT>::Ptr> &,
        pcl::PointCloud<PointT>::Ptr, const std::vector<int> &);


 protected:
    pcl::PointCloud<PointT>::Ptr input_cloud;
    pcl::PointCloud<pcl::Normal>::Ptr surface_normal;

    void onInit();
    void subscribe();
    void unsubscribe();

    cv::Mat image_;
    
 public:
    PointCloudSceneDecomposer();
    void cloudCallback(
        const sensor_msgs::PointCloud2ConstPtr &);
    void pclDistanceFilter(
        const boost::shared_ptr<pcl::PCLPointCloud2>,
        pcl::PCLPointCloud2 &);
    void estimatePointCloudNormals(
        const pcl::PointCloud<PointT>::Ptr,
        pcl::PointCloud<pcl::Normal>::Ptr,
        const int = 8,
        const double = 0.03,
        bool = true);
    void pointCloudLocalGradient(
        const pcl::PointCloud<PointT>::Ptr,
        const pcl::PointCloud<pcl::Normal>::Ptr,
        cv::Mat &);
    void viewPointSurfaceNormalOrientation(
        pcl::PointCloud<PointT>::Ptr,
        const pcl::PointCloud<pcl::Normal>::Ptr,
        cv::Mat &);
    void extractPointCloudClustersFrom2DMap(
        const pcl::PointCloud<PointT>::Ptr,
        const std::vector<cvPatch<int> > &,
        std::vector<pcl::PointCloud<PointT>::Ptr> &,
        std::vector<pcl::PointCloud<pcl::Normal>::Ptr> &,
        pcl::PointCloud<pcl::PointXYZ>::Ptr);

    void imageCallback(
        const sensor_msgs::Image::ConstPtr &);
    
};
#endif  // _POINT_CLOUD_SCENE_DECOMPOSER_H_