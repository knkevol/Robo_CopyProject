# 🤖 *Robo Copy Project*
UE5와 C++을 기반으로 게임 로보퀘스트의 기능을 모작한 네트워크 FPS RPG 프로젝트
<br />
<br />
<br />
***
### ✅ 주요 기능

- **Unreal Engine 5 + C++ 기반 멀티플레이 액션 RPG 시스템 구현 (Listen Server)**
    
    RPC(Server/Client/Multicast) 및 Replication을 활용하여 서버 권한 기반 전투·성장·장비 상태를 동기화하고, Delegate Broadcast를 통한 이벤트 기반 구조 설계
    
- **Behavior Tree + Blackboard 기반 몬스터 AI 시스템 구현**
    
    Idle/Chase/Attack/Death 상태 전이 구조 설계 및 서버 권한 데미지 처리 로직 구성
    
- **상태 기반 동적 UI 인터페이스 설계 및 최적화**
    
    상호작용 대상에 따라 입력 가이드 및 전투/스탯 정보를 동적으로 표시

+ **Roboquest의 전투 구조를 분석하여 FPS 전투 시스템과 데이터 기반 장비 적용 구현**

  전투, , 맵 진입 시 몬스터 스폰, 서버-클라이언트 동기화 구조 설계 및 구현
  
+  **Trigger 기반 전투 구역 및 보스전 제어 시스템 구현**
    
    구역 진입 시 몬스터 Spawn, 조건 충족 시 다음 구역 개방, 보스 소환 시 전용 HP UI 동적 생성
    
- **전투 보상 연동 성장 시스템 구현**
    
    몬스터 처치 이벤트 기반 XP 누적, 레벨업 판정 및 능력치 반영, 레벨업 위젯 생성 및 스킬 선택 UI 흐름 제어
    
- **DataTable 기반 데이터테이블 아이템·장비 시스템 구현**
    
    무기 Equip 시스템, 랜덤 아이템 추출 로직 및 장비 데이터 위젯바인딩, 장착 상태 서버 동기화 및 UI 반영
    
- **애니메이션 이벤트 연동 전투 메커니즘 구현**
    
    AnimNotify 기반 재장전 완료 시 탄약 갱신 및 UI 실시간 업데이트 처리
+ **HTTP 기반 서버 인증 로그인 시스템 구현**
  
  UE C++ HTTP 통신을 통한 REST API 연동 및 MySQL(Docker) 기반 계정 검증 구조 설계
<br />

---

### 📂 프로젝트 구조
    Robo_CopyProject/
      └── Source/
        └── Robo_CopyProject/
            ├── AnimNotify/
            ├── Cemetery/
            ├── Interface/
            ├── Item/
            ├── Login/
            ├── Main/
            ├── MapActor/
            ├── Monster/
            ├── Player/
            ├── Weapon/
            ├── Widget/
            ├── Robo_CopyProject.Build.cs
            ├── Robo_CopyProject.cpp
            └── Robo_CopyProject.h
<br />

---

### 🧩 기술 스택

| 구분 | 기술 |
|------|------|
| Engine | Unreal Engine 5 |
| Language | C++ |
| IDE | Visual Studio 2022 |
| Database | MySQL |
| Networking | HTTP / REST API |
