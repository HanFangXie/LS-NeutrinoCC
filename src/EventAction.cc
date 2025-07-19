#include "EventAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

namespace B1 {

EventAction::EventAction() {
    outFile.open("event_data.txt");
    outFile << "ElectronEdep(MeV)     ClusterCount     GammaDetected\n";
//    outFile << "EventID     ElectronEdep(MeV)     ClusterCount     GammaDetected\n";

}

// 必须添加虚析构函数实现
EventAction::~EventAction() {
    if(outFile.is_open()) {
        outFile.close();
    }
}

void EventAction::BeginOfEventAction(const G4Event*) {
    // 重置所有临时变量
    totalElectronEdep = totalPositronEdep = 0.;
    electronPos.isSet = positronPos.isSet = false;
    electronHits.clear();
    gammaEnergies.clear();
    hasDetectableGamma = false;
    clusterCount = 0;
}

void EventAction::AddGammaEnergy(double energy) {
//    G4cout << "Gamma energy received: " << energy/MeV << " MeV" << G4endl;
    gammaEnergies.push_back(energy);
    if (energy >= 2*MeV && energy <= 7*MeV) {
        hasDetectableGamma = true;
    }
}

void EventAction::EndOfEventAction(const G4Event* event) {
    // 簇射分析算法
    const double CLUSTER_THRESHOLD = 50*mm;   //set 10 mm  // 空间分割阈值
    if (!electronHits.empty()) {
        clusterCount = 1;   // 初始化为单簇射
        for (size_t i = 1; i < electronHits.size(); ++i) {
          // 计算相邻沉积点的距离
            if ((electronHits[i] - electronHits[i-1]).mag() > CLUSTER_THRESHOLD) {
                clusterCount++;   // 超过阈值则判定为新簇射
            }
        }
    }

    // 数据输出
    outFile // << event->GetEventID() << "         "  // 记录事件ID
            << totalElectronEdep/MeV << "         "
            << clusterCount << "         "
            << hasDetectableGamma << "\n";

    if (!outFile) {
          G4cerr << "!!! File write error at event " << event->GetEventID() << " !!!" << G4endl;
    }

}

// 位置记录实现
void EventAction::SetElectronPos(double x, double y, double z) {
    if (!electronPos.isSet) {
        electronPos = {x, y, z, true};
    }
}

void EventAction::SetPositronPos(double x, double y, double z) {
    if (!positronPos.isSet) {
        positronPos = {x, y, z, true};
    }
}

void EventAction::AddElectronHit(double x, double y, double z) {
    electronHits.emplace_back(x, y, z);
}

} // namespace B1
