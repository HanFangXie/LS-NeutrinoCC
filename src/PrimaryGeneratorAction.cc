#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include <cmath>
#include "G4RandomDirection.hh"

namespace B1 {

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr),
   electron(nullptr),
   gamma(nullptr)
{
    fParticleGun = new G4ParticleGun(1);
    electron = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    gamma = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

double PrimaryGeneratorAction::SampleElectronEnergy()
{
    // 能量点（MeV）和相对概率
    const G4int nBins = 9;
    G4double energies[nBins] = {18,20,22,24,26,28,30,32,34};
    G4double weights[nBins] = {0.85,0.95,1.0,0.98,0.90,0.75,0.55,0.30,0.10};

    // 逆变换采样
    G4double rnd = G4UniformRand();
    for (G4int i=0; i<nBins; ++i) {
        if (rnd <= weights[i]) {
            return energies[i];
        }
        rnd -= weights[i];
    }
    return 22.0; // 默认返回峰值能量
}

G4ThreeVector PrimaryGeneratorAction::SampleElectronDirection()
{
    // 1. 相对于中微子方向的cosθ（论文图18特征）
    double cos_theta = -0.25 + 0.5*std::pow(G4UniformRand(), 1.5);

    // 2. 随机方位角
    double phi = 2*M_PI*G4UniformRand();

    // 3. 转换为方向矢量
    double sin_theta = std::sqrt(1. - cos_theta*cos_theta);
    return G4ThreeVector(
        sin_theta * std::cos(phi),
        sin_theta * std::sin(phi),
        cos_theta
    );
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    // 1. 硬编码GdLS圆柱参数（单位：m）
    const G4double gdlsRadius = 0.98 *m;  // 半径（考虑1cm安全距离）
    const G4double gdlsLength = 0.98 *m;   // 总长度（沿X轴）

    // 2. 圆柱坐标系内随机抽样
    G4double r = gdlsRadius * std::sqrt(G4UniformRand());  // 半径均匀分布
    G4double phi = 2*M_PI * G4UniformRand();               // 方位角
    G4double x = (G4UniformRand() - 0.5) * gdlsLength;     // 轴向位置

    // 3. 转换为笛卡尔坐标（圆柱沿X轴）
    G4ThreeVector position(
        x,  // X轴（轴向）
        r * std::cos(phi),  // Y轴
        r * std::sin(phi)   // Z轴
    );

  /*  // 4. 调试输出（单位：m）
    G4cout << "Generated at ("
           << position.x()/m << ", "
           << position.y()/m << ", "
           << position.z()/m << ") m" << G4endl;
*/
    // 5. 设置粒子枪
    fParticleGun->SetParticlePosition(position);
    fParticleGun->SetParticleDefinition(electron);
    fParticleGun->GeneratePrimaryVertex(event);

    // 6. 生成伴随γ（同位置）
    GenerateGammaPrimaries(event, position);
}

void PrimaryGeneratorAction::GenerateGammaPrimaries(G4Event* event, const G4ThreeVector& pos) {
    int nGamma = CLHEP::RandPoisson::shoot(1.5);
    for (int i=0; i<nGamma; ++i) {
        fParticleGun->SetParticleDefinition(gamma);
        fParticleGun->SetParticlePosition(pos);  // 使用传入的位置
        fParticleGun->SetParticleEnergy((2 + 5*G4UniformRand())*MeV);
        fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
        fParticleGun->GeneratePrimaryVertex(event);
    }
}


} // namespace B1
