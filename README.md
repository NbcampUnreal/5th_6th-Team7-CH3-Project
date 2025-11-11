 # Z-WAVE
 <img width="1024" height="712" alt="image" src="https://github.com/user-attachments/assets/6caec64d-4a84-431b-bf93-3f8083e29b17" />
 
<p align="center">
  언리얼 기반 3D TPS / 웨이브 디펜스 프로젝트  
  <br/>
  팀 스파르타 / Team7 CH3
</p>
  <br/>
 <p align="center"><strong>정화장치를 향해 다가오는 좀비들을 막아내어 생존하라!</strong></p>
  <br/>

**Z-Wave**는 4방향에서 정화장치를 파괴하기 위해, 혹은 플레이어를 공격하기 위해 몰려오는 몬스터를 터렛과 강화된 다양한 종류의 무기로 웨이브를 막아내는 디팬스 게임입니다.

플레이어는 몬스터를 잡아 재화를 획득하여 플레이어의 능력치를 강화하고, 무기를 제조하거나 강화할 수 있으며, 혹은 다양한 종류의 투척물을 구매하여 몬스터를 막아낼 수 있습니다.

몰려오는 몬스터들을 혼자 감당하기 힘들다면 터랫을 설치하여 몬스터의 주의를 돌리고, 공격을 보조하게 할 수 있습니다.

- 장르: 3D TPS / 디펜스
- 개발 엔진: Unreal Engine 5.5
- 개발 기간: 2025.10.20 ~ 2025.11.07
- 인원: 6명


## 주요 시스템

### 1. 전투 시스템

- TPS 시점의 슈팅 전투
- 무기 교체 / 조준 / 재장전 등
- 적 AI: 시야, 추적, 공격, 웨이브 기반 스폰

### 2. 적 AI & 웨이브 시스템

- Behavior Tree / BlackBoard 기반의 AI 구조
- 스폰 지점별 웨이브 패턴 관리
- WaveManager / SpawnManager 서브시스템

### 3. 맵 & 레벨 디자인

- 1층/2층 구조, 계단을 활용한 입체적인 전투
- 생채바닥(파괴 가능한 바닥)을 통하여 몬스터의 진행을 변경
- 개폐 가능한 문을 통하여 몬스터의 진로를 방해할 수 있음

### 4. 상호작용 시스템

- 개폐 가능한 문을 플레이어와 상호작용을 통하여 동작

  
### 5. 상점 시스템

- 전투 준비 중 UI 상호작용을 통하여 준비
- 능력치 강화 기능
- 설치 기능
- 장비 제작, 강화 기능
- 장비 효과 부여 기능


## [UML](https://www.figma.com/board/GDLUIai8r7xff9ryk6OwAV/UML-diagram--Community-?node-id=0-1&p=f&t=3QhU7qPrTz2ZGqCS-0)

  <br/>
  <br/>
  
## 사용 기술 스택

### 🔹 Engine & Language

- Unreal Engine 5.5
- C++

### 🔹 Game Programming

- Behavior Tree / Blackboard
- AIController / NavMesh
- Animation Blueprint / Montage

### 🔹 협업 & 형상 관리

- Git / GitHub

  
## 프로젝트 구조

Z-WAVE PROJECT <br/>
 ┣ Animation <br/>
 ┃ ┗ AnimNotify <br/>
 ┣ AoE   <br/>
 ┣ Base <br/>
 ┣ BioLogical <br/>
 ┣ DamageCalculator <br/>
 ┣ Effect <br/>
 ┣ Enemy <br/>
 ┃ ┣ BT <br/>
 ┃ ┃ ┣ Decorator <br/>
 ┃ ┃ ┣ Service <br/>
 ┃ ┃ ┗ Task <br/>
 ┣ FieldItem <br/>
 ┣ Item <br/>
 ┣ Level <br/>
 ┣ Mode <br/>
 ┣ Player <br/>
 ┣ Prop <br/>
 ┣ Shop <br/>
 ┣ State <br/>
 ┣ UI <br/>
 ┗ Weapon <br/>

## 상세 설명

[캐릭터 이동 / 상태 변환](https://www.notion.so/teamsparta/2a52dc3ef5148059a079de23770847e7)<br/>
[애니메이션 리타게팅](https://www.notion.so/10-20-10-24-2966e3215b3a80469443d5c4fddbf0d9)<br/>
[원거리 몬스터](https://www.notion.so/teamsparta/2a52dc3ef514808ea3c8e14ee5db3273)<br/>
[벽타기 몬스터](https://www.notion.so/teamsparta/2a52dc3ef51480aabea1eb4da3bf226d)<br/>
[인벤토리와 Item, Weapon](https://www.notion.so/teamsparta/WeaponActor-2a72dc3ef51480fea9ced1a19d8b9523)<br/>
[UI](https://iris-capri-7ee.notion.site/CH03-2a5ec2a939d4801fb77de53892b8f67e?pvs=74)<br/>
[AI](https://www.notion.so/teamsparta/Enemy-2a72dc3ef51480c3ae95da1125538157)  
[게임효과와Damage](https://www.notion.so/Effect-Damage-2a7ed759532f80b3a8f0dec4ec8f0f8d?source=copy_link)<br/>
