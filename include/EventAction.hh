#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include <fstream>
#include <vector>

namespace B1 {

class EventAction : public G4UserEventAction {
public:
    // 构造函数与析构
    EventAction();
    virtual ~EventAction();

    // 核心方法   必须实现基类虚函数
    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

    // 电子数据记录
    void AddElectronEdep(double edep) { totalElectronEdep += edep; }
    void SetElectronPos(double x, double y, double z);
    bool IsElectronPosSet() const { return electronPosSet; }
    void AddElectronHit(double x, double y, double z);

    // 正电子数据记录
    void AddPositronEdep(double edep) { totalPositronEdep += edep; }
    void SetPositronPos(double x, double y, double z);
    bool IsPositronPosSet() const { return positronPosSet; }

    // 新增γ探测功能
    void AddGammaEnergy(double energy);

private:
    // 能量沉积
    double totalElectronEdep = 0.;
    double totalPositronEdep = 0.;

    // 位置信息
    struct PositionRecord {
        double x, y, z;
        bool isSet = false;
    } electronPos, positronPos;

    // 簇射分析
    std::vector<G4ThreeVector> electronHits;
    int clusterCount = 0;

    // γ探测
    std::vector<double> gammaEnergies;
    bool hasDetectableGamma = false;

    // 位置记录状态
    bool electronPosSet = false;  // 新增
    bool positronPosSet = false;  // 新增

    // 数据输出
    std::ofstream outFile;
};

} // namespace B1
#endif
