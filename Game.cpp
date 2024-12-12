#include "Game.h"
#include "BladeWhirl.h"
#include "BulkUp.h"
#include "Teleport.h"
#include "ArrowTower.h"
#include "WizardTower.h"
#include "TrainingTower.h"
#include "BombTower.h"
#include "Dash.h"
#include "CannonTower.h"
#include "BombTower.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
Game::Game() :
    window(sf::VideoMode(1600, 1000), "Warrior and Monsters"),
    warrior("knight.png", 600, 500, 1.0f, 100.0f),
    uiManager(font, &warrior, window),
    minimap(600, 600, 0.4f),
    mainView(sf::FloatRect(0, 0, 1600, 1000)),
    mainTower(sf::Vector2f(650.0f, 500.0f)), // MainTower 위치 설정
    towerView(sf::FloatRect(150, 0, 1000, 1000)),
	setSubTowerView(sf::FloatRect(0, 0, 1600, 1000)),
    spawnInterval(1.0f),
    monsterSpeed(50.0f),
    experience(0),
    level(1),
    experienceToNextLevel(100),
    waveManager(&warrior, &mainTower, &monsters, 1600, 1600),
    skillManager(),
    upgradeManager(&warrior, &mainTower,&skillManager,&subTowerManager,level),
    upgradeUI(font, sf::Vector2f(window.getSize())),
    screenUI(sf::Vector2f(window.getSize())),  // Add ScreenUI initialization
    isGameOver(false),
    mainBossDefeated(false),
    isVictory(false)
{
    minimap.setPosition(3, 3);  // 기본 미니맵 위치 설정
    font.loadFromFile("arial.ttf");
	pixelFont.loadFromFile("PixelOperator8.ttf");  
	backgroundTexture.loadFromFile("background.png");
    bossbackgroundTexture.loadFromFile("Bossbackground.png");
	backgroundEdgeTexture.loadFromFile("backgroundEdge.png");
	backgroundEdgeSprite.setTexture(backgroundEdgeTexture);
	backgroundSprite.setTexture(backgroundTexture);
    screenUI.loadResources("StartUi.png", "PixelOperator8.ttf");
    cacheBackground();
	setSubTowerView.setCenter(mainTower.getPosition());
    gameStarted = false;
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        // Check if game has started
        if (!gameStarted) {
            // Update and render only the start screen
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            screenUI.update(mousePos);
            window.clear();
            screenUI.draw(window);
            window.display();
            // Check for start button click
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (screenUI.handleClick(mousePos)) {
                    gameStarted = true;
                    clock.restart();
                }
            }
            continue;  // Skip main game update and render
        }      
        update();
        // Normal game update and render
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}
void Game::update() {
    if (!gameStarted || isGameOver || isVictory) {
        return;
    }
    // Game Over 조건
    if (mainTower.getHealth() <= 0 || warrior.getHealth() <= 0) {
        //isGameOver = true;
       // screenUI.setGameOver(true);
    }
    else if (mainBossDefeated) {
        isVictory = true;
        screenUI.setVictory(true);  // ScreenUI에 victory 상태 설정
        return;
    }

    if (upgradeUI.getIsVisible()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int choice = upgradeUI.handleClick(mousePos);
            if (choice != -1&&!selectingTower) {
                upgradeManager.applyUpgrade(choice); // 선택된 업그레이드 적용
                upgradeUI.hide(); // UI 숨기기 (업그레이드 완료)
                clock.restart();
            }
			else if (choice != -1 && selectingTower) {
				towerType = upgradeManager.getTowerName(choice); // 선택된 업그레이드 적용
				upgradeUI.hide(); // UI 숨기기 (업그레이드 완료)
				clock.restart();
			}
        }
        return; // 업그레이드 UI가 활성화된 동안 다른 게임 업데이트 중단
    }
	
    if (selectingTower&&!upgradeUI.getIsVisible()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&&!wasMousePressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			if (calculateDistance(mousePos, mainTower.getPosition()) < 300|| calculateDistance(mousePos, mainTower.getPosition()) >500||!subTowerManager.isCorrectPos(mousePos)) return;
            if (towerType == "ArrowTower")subTowerManager.addTower(std::make_unique<ArrowTower>(mousePos));
            else if (towerType == "WizardTower")subTowerManager.addTower(std::make_unique<WizardTower>(mousePos));
            else if (towerType == "TrainingTower")subTowerManager.addTower(std::make_unique<TrainingTower>(mousePos));
            else if (towerType == "BombTower")subTowerManager.addTower(std::make_unique<BombTower>(mousePos));
            else if (towerType == "CannonTower")subTowerManager.addTower(std::make_unique<CannonTower>(mousePos));
            selectingTower = false;
            clock.restart();
		}
        wasMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		return; // 업그레이드 UI가 활성화된 동안 다른 게임 업데이트 중단
        
    }
    deltaTime = clock.restart().asSeconds();
    warrior.handleInput(deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        skillManager.activateSkill(sf::Keyboard::Q); // Q스킬 활성화
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        skillManager.activateSkill(sf::Keyboard::W); // W스킬 활성화
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        skillManager.activateSkill(sf::Keyboard::E); // E스킬 활성화
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        skillManager.activateSkill(sf::Keyboard::R); // R스킬 활성화
    }
    warrior.updateAnimation(deltaTime);


    if (warrior.getIsSwinging() && !warrior.getAttackApplied()) {
        warrior.basicAttack(monsters);

        warrior.setAttackApplied(true);  // 공격 적용 완료 플래그 설정
    }
    // 체력이 0 이하인 몬스터들을 한 번에 제거
    monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
        [this](const std::unique_ptr<Monster>& monster) {
            if (monster->getHealthPoint() <= 0) {
                if (monster->getBossMonsterType() == MonsterType::Main_Boss) {
                    mainBossDefeated = true;  // 메인 보스 처치 표시
                    printf("Main Boss has been defeated!\n");
                }
                addExp(30);
            }
            return monster->getHealthPoint() <= 0;
        }),
        monsters.end());
    if (experience >= experienceToNextLevel) {
        onLevelUp();
    }

    sf::Vector2f warriorPosition = warrior.getPosition();
    /*for (auto& monster : monsters) {
        monster.update(warriorPosition, deltaTime);
    }*/
    waveManager.update(deltaTime);
    mainView.setCenter(warriorPosition);

    minimap.update(towerView, mainTower, monsters, warrior);

    minimap.setPosition(mainView.getCenter().x - mainView.getSize().x / 2 + 3,
        mainView.getCenter().y + mainView.getSize().y / 2 - 243);
    uiManager.updateCharacterHealth();
    uiManager.updateLevelBar(level, experience, experienceToNextLevel);
    uiManager.updateTimer(deltaTime);
    uiManager.updateTowerDurability(mainTower.getHealth(), mainTower.getMaxHealth());
    mainTower.healNearbyCharacter(deltaTime, warrior);
    skillManager.updateSkills(deltaTime);
    subTowerManager.updateTowers(monsters,deltaTime);
    
}

void Game::render() {
    window.clear();
    if (isGameOver) {
        window.setView(window.getDefaultView());
        screenUI.draw(window);
    }
    else if (isVictory) {
        window.setView(window.getDefaultView());
        screenUI.draw(window);
    }
    /*else if (upgradeUI.getIsVisible()) {
        window.setView(window.getDefaultView());  // 기본 뷰로 변경
        upgradeUI.draw(window); // UI가 활성화된 경우에만 그리기
    }*/
    else {
        if (waveManager.isBossSpawned()&&isBackgroundCached==false) {
            backgroundSprite.setTexture(bossbackgroundTexture);
			cacheBackground();
			isBackgroundCached = true;
        }
        
        

        window.setView(mainView);
        window.draw(cachedBackgroundSprite);
        window.draw(towerSprite);

        mainTower.draw(window);
        subTowerManager.drawTowers(window);
        waveManager.drawMonsters(window);
        warrior.draw(window);
        if (skillManager.hasSkill("BladeWhirl")) {
            BaseSkill* skill = skillManager.getSkill("BladeWhirl");
            // dynamic_cast로 BladeWhirl로 안전하게 변환
            BladeWhirl* bladeWhirl = dynamic_cast<BladeWhirl*>(skill);
            if (bladeWhirl) {
                bladeWhirl->draw(window);
            }
        }

        // 미니맵 그리기
        minimap.draw(window);
        uiManager.draw(window);// UI 그리기
        uiManager.updateSkillIconPositions();
        uiManager.updateSkillCoolTime(skillManager);
		
    }
    if (upgradeUI.getIsVisible()) {
        window.setView(window.getDefaultView());  // 기본 뷰로 변경
        upgradeUI.draw(window); // UI가 활성화된 경우에만 그리기
    }
    // 타워 설치 모드일 때 UI 요소 렌더링
    if (selectingTower && !upgradeUI.getIsVisible()) {
        window.setView(setSubTowerView); // 기본 뷰로 변경
        window.draw(cachedBackgroundSprite);
		mainTower.draw(window);
		subTowerManager.drawTowers(window);
        sf::CircleShape wrongPlace;
        sf::CircleShape rightPlace;
        wrongPlace.setRadius(300);
        wrongPlace.setOrigin(300, 300);
        wrongPlace.setPosition(mainTower.getPosition());
        wrongPlace.setFillColor(sf::Color(255, 0, 0, 128));
        rightPlace.setRadius(500);
        rightPlace.setOrigin(500, 500);
        rightPlace.setPosition(mainTower.getPosition());
        rightPlace.setFillColor(sf::Color(0, 255, 0, 128));
        window.draw(rightPlace);
        window.draw(wrongPlace);
        sf::Text instructionText;
        instructionText.setFont(font);
        instructionText.setString("Click on the map to place your " + towerType);
        instructionText.setCharacterSize(24);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(50, 50);
        window.draw(instructionText);

        // 타워 미리보기 (선택 사항)
        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos, window.getView());

        sf::Sprite previewSprite;
        if (towerType == "WizardTower") {
            previewSprite.setTexture(ResourceManager::getInstance().getTexture("WizardTower"));
        }
        else if (towerType == "ArrowTower") {
            previewSprite.setTexture(ResourceManager::getInstance().getTexture("ArrowTower"));
        }
        else if (towerType == "BombTower") {
            previewSprite.setTexture(ResourceManager::getInstance().getTexture("BombTower"));
        }
        else if (towerType == "TrainingTower") {
            previewSprite.setTexture(ResourceManager::getInstance().getTexture("TrainingTower"));
        }
        else if (towerType == "CannonTower") {
            previewSprite.setTexture(ResourceManager::getInstance().getTexture("CannonTower"));
        }
        else {
            // 기본 처리 또는 에러 메시지
        }
        previewSprite.setPosition(mouseWorldPos);
        previewSprite.setColor(sf::Color(255, 255, 255, 128)); // 반투명
        if (calculateDistance(previewSprite.getPosition(), mainTower.getPosition()) < 300 || calculateDistance(previewSprite.getPosition(), mainTower.getPosition()) > 500||!subTowerManager.isCorrectPos(previewSprite.getPosition())) {
			previewSprite.setColor(sf::Color(255, 0, 0, 128)); // 빨간색으로 변경
        }
        previewSprite.setOrigin(previewSprite.getTexture()->getSize().x / 2.0f, previewSprite.getTexture()->getSize().y / 2.0f);
        window.draw(previewSprite);
    }
    window.display();
}
/*
void Game::spawnMonster() {
    float x = static_cast<float>(std::rand() % window.getSize().x);
    float y = static_cast<float>(std::rand() % window.getSize().y);
    monsters.emplace_back(x, y, monsterSpeed);
}*/
void Game::addExp(float exp) {
    experience += exp;
}
void Game::onLevelUp() {
    experience -= experienceToNextLevel;
    if (experienceToNextLevel * 0.2 > 100) {
        experienceToNextLevel += 100;
    }
    else{
        experienceToNextLevel *= 1.2;
    }
    level += 1;
    if (level == 2) {
        skillManager.unlockSkill("BladeWhirl");
        skillManager.addSkill("BladeWhirl", std::make_unique<BladeWhirl>(&warrior, monsters));
        /*subTowerManager.addTower(std::make_unique<WizardTower>(sf::Vector2f(350, 326)));
        subTowerManager.addTower(std::make_unique<BombTower>(sf::Vector2f(950, 326)));
        subTowerManager.addTower(std::make_unique<TrainingTower>(sf::Vector2f(650, 846)));

        subTowerManager.addTower(std::make_unique<ArrowTower>(sf::Vector2f(250, 846)));
        subTowerManager.addTower(std::make_unique<CannonTower>(sf::Vector2f(1050, 846)));*/
    }

    if (level == 4) {
        skillManager.unlockSkill("BulkUp");
        skillManager.addSkill("BulkUp", std::make_unique<BulkUp>(&warrior));
    }
    if (level == 6) {
        skillManager.unlockSkill("Dash");
		skillManager.addSkill("Dash", std::make_unique<Dash>(&warrior));
    }
    if (level == 8) {
        skillManager.unlockSkill("Teleport");
        skillManager.addSkill("Teleport", std::make_unique<Teleport>(&warrior,&mainTower));
    }
	if (level == 3 || level == 5 || level == 7) {
		upgradeManager.createSubTowerOptions(); // 서브 타워 옵션 생성
		std::vector<std::string> options = upgradeManager.getUpgradeDescriptions();
		std::vector<std::string> imagePaths = upgradeManager.getUpgradeImagePaths();
		upgradeUI.showOptions(options, imagePaths); // UI에 업그레이드 옵션 표시
        selectingTower = true;
	}
    else {
        upgradeManager.generateUpgradeOptions(); // 업그레이드 옵션 생성
        std::vector<std::string> options = upgradeManager.getUpgradeDescriptions();
	    std::vector<std::string> imagePaths = upgradeManager.getUpgradeImagePaths();
        upgradeUI.showOptions(options,imagePaths); // UI에 업그레이드 옵션 표시
    }
    
}
/*
void Game::loadResources() {
    ResourceManager& rm = ResourceManager::getInstance();
    rm.loadTexture("ArrowTower", "ArrowTower.png");
    rm.loadTexture("WizardTower", "WizardTower.png");
    rm.loadTexture("TrainingTower", "TrainingTower.png");
    rm.loadTexture("BombTower", "BombTower.png");
	rm.loadTexture("CannonTower", "CannonTower.png");
    rm.loadTexture("SpeedMonster", "speedMonster.png");
    rm.loadTexture("AttackMonster", "attackMonster.png");
    rm.loadTexture("BasicMonster", "basicMonster.png");
    rm.loadTexture("DefenseMonster", "defenseMonster.png");
    rm.loadTexture("MainBoss", "mainboss.png");
    rm.loadTexture("MidBoss", "midboss.png");
    rm.loadTexture("BossExplode", "BossExplode.png");
	rm.loadTexture("GreenBomb", "greenBomb.png");
	rm.loadTexture("GreenBombExplode", "greenBombExplode.png");
	rm.loadTexture("PinkBomb", "pinkBomb.png");
	rm.loadTexture("PinkBombExplode", "pinkBombExplode.png");
	rm.loadTexture("YellowBomb", "yellowBomb.png");
	rm.loadTexture("YellowBombExplode", "yellowBombExplode.png");
	rm.loadTexture("BlueBomb", "blueBomb.png");
	rm.loadTexture("BlueBombExplode", "blueBombExplode.png");
    rm.loadTexture("RedPen", "RedPen.png");
	rm.loadTexture("OrangePen", "OrangePen.png");
	rm.loadTexture("YellowPen", "YellowPen.png");
	rm.loadTexture("GreenPen", "GreenPen.png");
	rm.loadTexture("BluePen", "BluePen.png");
	rm.loadTexture("NavyPen", "NavyPen.png");
	rm.loadTexture("PurplePen", "PurplePen.png");
	rm.loadTexture("pencil", "pencil.png");
	rm.loadImage("MainBossYellowSkill", "MainBossYellowSkill.png");
	rm.loadImage("MainBossRedSkill", "MainBossRedSkill.png");
	rm.loadImage("MainBossSkillPurple", "MainBossSkillPurple.png");
    rm.loadImage("MainBossPencilSkill", "MainBossPencilSkill.png");
	rm.loadTexture("UpgradeArrowTower", "UpgradeArrowTower.png");
	rm.loadTexture("UpgradeWizardTower", "UpgradeWizardTower.png");
	rm.loadTexture("UpgradeTrainingTower", "UpgradeTrainingTower.png");
	rm.loadTexture("UpgradeBombTower", "UpgradeBombTower.png");
	rm.loadTexture("UpgradeCannonTower", "UpgradeCannonTower.png");
	rm.loadTexture("UpgradeAttackPower", "UpgradeAttackPower.png");
	rm.loadTexture("UpgradeHealth", "UpgradeHealth.png");
	rm.loadTexture("UpgradeBasicCooldown", "UpgradeBasicCooldown.png");
	rm.loadTexture("UpgradePlayerSpeed", "UpgradePlayerSpeed.png");
	rm.loadTexture("UpgradeBladeWhirl", "UpgradeBladeWhirl.png");
	rm.loadTexture("UpgradeBulkUp", "UpgradeBulkUp.png");
	rm.loadTexture("UpgradeDash", "UpgradeDash.png");
    rm.loadTexture("QSkillUI", "QSkillUI.png");
	rm.loadTexture("WSkillUI", "WSkillUI.png");
	rm.loadTexture("ESkillUI", "ESkillUI.png");
	rm.loadTexture("RSkillUI", "RSkillUI.png");
	rm.loadTexture("SkillLock", "SkillLock.png");
    rm.loadFont("Arial", "arial.ttf");
	rm.loadFont("Pixel", "PixelOperator8.ttf");
}*/
void Game::cacheBackground() {
    // 최종 크기: 5800 x 6000
    backgroundCache.create(5800, 6000);
    backgroundCache.clear();

    // 기존 배경 크기: 4800 x 5000
    // 타일 크기: 300 x 200
    // 가로 타일 수: 17, 세로 타일 수: 26
    int horizontalTiles = 17;
    int verticalTiles = 26;

    // 기존 배경을 중앙에 배치하기 위해 (500, 500) 위치부터 그리기 시작
    for (int i = 0; i < horizontalTiles; ++i) {
        for (int j = 0; j < verticalTiles; ++j) {
            backgroundSprite.setPosition(500 + i * 300, 500 + j * 200);
            backgroundCache.draw(backgroundSprite);
        }
    }

    // 이제 가장자리에 backgroundEdgeSprite를 배치
    // backgroundEdgeSprite 크기: 100 x 500
    // 상단과 하단의 폭: 5800, 따라서 100픽셀짜리 스프라이트 58개 필요
    int edgeCountHoriz = 5800 / 100; // 58개

    // 상단 가장자리 (y=0)
    for (int i = 0; i < edgeCountHoriz; ++i) {
        backgroundEdgeSprite.setPosition(i * 100, 0);
        backgroundCache.draw(backgroundEdgeSprite);
    }

    // 하단 가장자리 (y=5500)
    for (int i = 0; i < edgeCountHoriz; ++i) {
        backgroundEdgeSprite.setPosition(i * 100, 5500);
        backgroundCache.draw(backgroundEdgeSprite);
    }

    // 좌우 가장자리
    // 전체 높이: 6000, 세로 타일링: 500 픽셀짜리를 12번 (6000/500=12)
    int edgeCountVert = 6000 / 500; // 12개

    // 왼쪽 가장자리 (x=0)
    for (int j = 0; j < edgeCountVert; ++j) {
		for (int i = 0; i < 5; i++) {
			backgroundEdgeSprite.setPosition(i * 100, j * 500);
			backgroundCache.draw(backgroundEdgeSprite);
		}
    }

    // 오른쪽 가장자리 (x=5700, 5800-100=5700)
    for (int j = 0; j < edgeCountVert; ++j) {
        for (int i = 0; i < 5; i++) {
			backgroundEdgeSprite.setPosition(5300 + i * 100, j * 500);
			backgroundCache.draw(backgroundEdgeSprite);
        }
    }

    // 렌더 완료
    backgroundCache.display();

    // RenderTexture의 텍스처를 Sprite에 설정
    cachedBackgroundSprite.setTexture(backgroundCache.getTexture());

    // 원래 위치를 감안한 오프셋 설정 (예: 기존 코드처럼)
    cachedBackgroundSprite.setPosition(-2100, -2500);
}

