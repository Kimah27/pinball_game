//-----------------------------------------------------------------------------
// MainApp.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include "MainApp.h"
#include "RTRShader.h"

int MainApp::Init()
{
    if (int err = RTRApp::Init() != 0)
    {
        return err;
    }

    // Setup projection matrix and viewport transform. 
    // These won't need to change as we're not worrying about screen size changes for this assignment
    m_ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    // You might want to maintain a vector of all shaders you will use in your assignment
    m_DefaultShader = new RTRShader();
    if (m_DefaultShader->Load("Src/RTRDefault.vert", "Src/RTRDefault.frag", "Src/RTRDefault.geom") != 0)
    {
        return -1;
    }

    // Create and initialise camera
    m_Camera = new RTRCamera(glm::vec3(0.0, 3.5, 8.0), glm::vec3(0.0, 1.0, 0.0), -90.0, -30.0);
    //m_Camera = new RTRCamera(glm::vec3(-8.0, 5.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 0.0, -30.0);

    // Create and initialise lighting model
    m_LightingModel = new RTRLightingModel();
    
    // Add directional light to follow camera view direction
    m_LightingModel->AddLight({
        .Type = RTRLightType::DirectionalLight,
        .Ambient = glm::vec3(0.2, 0.2, 0.2),
        .Diffuse = glm::vec3(1.0, 1.0, 1.0),
        .Specular = glm::vec3(0.5, 0.5, 0.5),
        .Direction = glm::vec3(1.0, 0.0, 0.0)
        });

    // Add x-axis red spot lights (place all at x+3.0 so they are positioned around shaded cube)
    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(1.0, 0.0, 0.0),
    //    .Diffuse = glm::vec3(1.0, 0.0, 0.0),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(-6.0, 3.0, -2.0),
    //    .Constant = 1.0f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //});

    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(0.0, 0.0, 1.0),
    //    .Diffuse = glm::vec3(0.0, 0.0, 1.0),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(-6.0, 3.0, 2.0),
    //    .Constant = 1.0f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });

    //// Add y-axis green spot lights
    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(0.0, 0.2, 0.0),
    //    .Diffuse = glm::vec3(0.0, 1.0, 0.0),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(0.0 + 3.0, 2.0, 0.0),
    //    .Constant = 1.0f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });

    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(0.0, 0.2, 0.0),
    //    .Diffuse = glm::vec3(0.0, 1.0, 0.0),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(0.0 + 3.0, -2.0, 0.0),
    //    .Constant = 1.0f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });

    //// Add z-axis blue spot lights
    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(0.0, 0.0, 0.2),
    //    .Diffuse = glm::vec3(0.0, 0.0, 1.0),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(0.0 + 3.0, 0.0, 2.0),
    //    .Constant = 1.0f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });

    //m_LightingModel->AddLight({
    //    .Type = RTRLightType::PointLight,
    //    .Ambient = glm::vec3(0.0, 0.0, 0.2),
    //    .Diffuse = glm::vec3(0.0, 0.0, 1.0),
    //    .Specular = glm::vec3(1.0, 1.0, 1.0),
    //    .Position = glm::vec3(0.0 + 3.0, 0.0, -2.0),
    //    .Constant = 1.0f,
    //    .Linear = 0.35f,
    //    .Quadratic = 0.44f
    //    });

    // You might want to maintain a vector of objects for your assignment
    table = new RTRObject();
    table->Init();
    table->m_Transform = glm::translate(table->m_Transform, glm::vec3(0.0f, 0.0f, 0.0f));
    table->m_Transform = glm::rotate(table->m_Transform, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0));
    table->IgnoreCollision = true;
    table->label = "Table";

    base = new RTRCube();
    base->Init(3.0f, 0.2f, 5.0f);
    base->IgnoreCollision = true;
    base->label = "Base";
    table->m_ChildObjects.push_back(base);
   
    topWall = new RTRCube();
    topWall->Init(3.0f, 0.2f, 0.1f);
    topWall->m_Transform = glm::translate(topWall->m_Transform, glm::vec3(0.0, 0.2, -5.1));
    topWall->label = "TopWall";
    table->m_ChildObjects.push_back(topWall);

    bottomWall = new RTRCube();
    bottomWall->Init(3.0f, 0.2f, 0.1f);
    bottomWall->m_Transform = glm::translate(bottomWall->m_Transform, glm::vec3(0.0, 0.2, 5.1));
    bottomWall->label = "BottomWall";
    table->m_ChildObjects.push_back(bottomWall);

    leftWall = new RTRCube();
    leftWall->Init(0.1f, 0.2f, 5.0f);
    leftWall->m_Transform = glm::translate(leftWall->m_Transform, glm::vec3(-3.1, 0.2, 0.0));
    leftWall->label = "LeftWall";
    table->m_ChildObjects.push_back(leftWall);

    rightWall = new RTRCube();
    rightWall->Init(0.1f, 0.2f, 5.0f);
    rightWall->m_Transform = glm::translate(rightWall->m_Transform, glm::vec3(3.1, 0.2, 0.0));
    rightWall->label = "rightWall";
    table->m_ChildObjects.push_back(rightWall);

    inLane = new RTRCube();
    inLane->Init(0.1f, 0.2f, 4.4f);
    inLane->m_Transform = glm::translate(inLane->m_Transform, glm::vec3(2.6, 0.2, 0.6));
    inLane->label = "InLane";
    table->m_ChildObjects.push_back(inLane);

    cornerWedge = new RTRCube();
    cornerWedge->Init(0.35f, 0.2f, 0.1f);
    cornerWedge->m_Transform = glm::translate(cornerWedge->m_Transform, glm::vec3(2.85, 0.2, -4.85));
    cornerWedge->m_Transform = glm::rotate(cornerWedge->m_Transform, glm::radians(-45.0f), glm::vec3(0.0, 1.0, 0.0));
    cornerWedge->label = "CornerWedge";
    table->m_ChildObjects.push_back(cornerWedge);

    rampL = new RTRCube();
    rampL->Init(0.1f, 0.2f, 1.5f);
    rampL->m_Transform = glm::translate(rampL->m_Transform, glm::vec3(-2.71, 0.2, 2.5));
    rampL->m_Transform = glm::rotate(rampL->m_Transform, glm::radians(15.0f), glm::vec3(0.0, 1.0, 0.0));
    rampL->label = "RampL";
    table->m_ChildObjects.push_back(rampL);

    rampL2 = new RTRCube();
    rampL2->Init(0.1f, 0.2f, 0.75f);
    rampL2->m_Transform = glm::translate(rampL2->m_Transform, glm::vec3(-2.3, 0.2, 4.0));
    rampL2->m_Transform = glm::rotate(rampL2->m_Transform, glm::radians(75.0f), glm::vec3(0.0, 1.0, 0.0));
    rampL2->label = "RampL2";
    table->m_ChildObjects.push_back(rampL2);

    //rampR = new RTRCube();
    //rampR->Init(0.1f, 0.2f, 1.5f);
    //rampR->m_Transform = glm::translate(rampR->m_Transform, glm::vec3(2.21, 0.2, 2.5));
    //rampR->m_Transform = glm::rotate(rampR->m_Transform, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
    //rampR->label = "RampR";
    //table->m_ChildObjects.push_back(rampR);

    rampR = new RTRFlipper();
    rampR->Init(0.1f, 0.2f, 1.5f);
    rampR->m_Transform = glm::translate(rampR->m_Transform, glm::vec3(2.21, 0.19, 2.5));
    rampR->m_Transform = glm::rotate(rampR->m_Transform, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
    rampR->label = "RampR";
    rampR->bounciness = 3.0f;
    rampR->SetMaterial(rampR->m_FlipperMaterial);
    table->m_ChildObjects.push_back(rampR);

    rampR2 = new RTRCube();
    rampR2->Init(0.1f, 0.2f, 0.75f);
    rampR2->m_Transform = glm::translate(rampR2->m_Transform, glm::vec3(1.8, 0.2, 4.0));
    rampR2->m_Transform = glm::rotate(rampR2->m_Transform, glm::radians(-75.0f), glm::vec3(0.0, 1.0, 0.0));
    rampR2->label = "RampR2";
    table->m_ChildObjects.push_back(rampR2);

    bumper = new RTRFlipper();
    bumper->Init(0.5f, 0.2f, 0.2f);
    bumper->m_Transform = glm::translate(bumper->m_Transform, glm::vec3(1.45, 0.2, -0.5));
    bumper->m_Transform = glm::rotate(bumper->m_Transform, glm::radians(-45.0f), glm::vec3(0.0, 1.0, 0.0));
    bumper->label = "Bumper";
    bumper->SetMaterial(bumper->m_FlipperMaterial);
    table->m_ChildObjects.push_back(bumper);

    plunger = new RTRFlipper();
    plunger->Init(0.1f, 0.2f, 0.2f);
    plunger->m_Transform = glm::translate(plunger->m_Transform, glm::vec3(2.85, 0.19, 5.0));
    plunger->label = "Plunger";
    plunger->SetMaterial(plunger->m_FlipperMaterial);
    table->m_ChildObjects.push_back(plunger);


    flipperLParent = new RTRObject();
    flipperLParent->Init();
    flipperLParent->m_Transform = glm::translate(flipperLParent->m_Transform, glm::vec3(-1.55f, 0.2f, 4.2f));
    flipperLParent->m_Transform = glm::rotate(flipperLParent->m_Transform, glm::radians(-10.0f), glm::vec3(0.0, 1.0, 0.0));
    flipperLParent->IgnoreCollision = true;
    flipperLParent->label = "FlipperLParent";
    table->m_ChildObjects.push_back(flipperLParent);

    flipperL = new RTRFlipper();
    flipperL->Init(0.5f, 0.2f, 0.1f);
    flipperL->m_Transform = glm::translate(flipperL->m_Transform, glm::vec3(0.5, 0.0, 0.0));
    flipperL->label = "FlipperL";
    flipperL->IsDynamic = true;
    flipperL->SetMaterial(flipperL->m_FlipperMaterial);
    flipperLParent->m_ChildObjects.push_back(flipperL);
    DynamicObjects.push_back(flipperL);


    flipperRParent = new RTRObject();
    flipperRParent->Init();
    flipperRParent->m_Transform = glm::translate(flipperRParent->m_Transform, glm::vec3(1.05, 0.2, 4.2));
    flipperRParent->m_Transform = glm::rotate(flipperRParent->m_Transform, glm::radians(10.0f), glm::vec3(0.0, 1.0, 0.0));
    flipperRParent->IgnoreCollision = true;
    flipperRParent->label = "FlipperLParent";
    table->m_ChildObjects.push_back(flipperRParent);

    flipperR = new RTRFlipper();
    flipperR->Init(0.5f, 0.2f, 0.1f);
    flipperR->m_Transform = glm::translate(flipperR->m_Transform, glm::vec3(-0.5, 0.0, 0.0));
    flipperR->label = "FlipperR";
    flipperR->IsDynamic = true;
    flipperR->SetMaterial(flipperR->m_FlipperMaterial);
    flipperRParent->m_ChildObjects.push_back(flipperR);
    DynamicObjects.push_back(flipperR);

    /*bumperCircleParent = new RTRObject();
    bumperCircleParent->Init();
    bumperCircleParent->m_Transform = glm::translate(bumperCircleParent->m_Transform, glm::vec3(-1.0, 0.2, 1.0));
    bumperCircleParent->IgnoreCollision = true;
    bumperCircleParent->label = "BumperCircleParent";
    table->m_ChildObjects.push_back(bumperCircleParent);*/
    
    bumperCircle = new RTRCylinder();
    bumperCircle->Init(0.4f);
    bumperCircle->m_Transform = glm::translate(bumperCircle->m_Transform, glm::vec3(-1.0, 0.2, 1.0));
    bumperCircle->label = "BumperCircle";
    bumperCircle->IsDynamic = true;
    bumperCircle->SetMaterial(bumperCircle->m_FlipperMaterial);
    table->m_ChildObjects.push_back(bumperCircle);
    DynamicObjects.push_back(bumperCircle);

    table->CalculateBoundingVolume(glm::mat4(1.0f));

    InitialiseUniformGrid();


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Assets/wood.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(2, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    int width2, height2, nrChannels2;
    unsigned char* data2 = stbi_load("Assets/wood_specular.jpg", &width2, &height2, &nrChannels2, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data2);

    //glGenTextures(3, &skybox);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //std::vector<std::string> faces
    //{
    //    "Assets/1.jpg",
    //    "Assets/2.jpg",
    //    "Assets/3.jpg",
    //    "Assets/4.jpg",
    //    "Assets/5.jpg",
    //    "Assets/6.jpg"
    //};

    //int width3, height3, nrChannels3;
    //for (unsigned int i = 0; i < 6; i++)
    //{
    //    unsigned char* data3 = stbi_load(faces[i].c_str(), &width3, &height3, &nrChannels3, 0);
    //    if (data3)
    //    {
    //        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
    //        stbi_image_free(data3);
    //    }
    //}
    

    
    // Create and initialise the debug console/overlay
    m_Console = new Console();
    m_Console->Init();

    return 0;
}

void MainApp::Done()
{
    table->End(); delete table;
    m_Console->End(); delete m_Console;
    delete m_DefaultShader;
    RTRApp::Done();
}

bool MainApp::Tick()
{
    m_QuitApp = false;
    CheckInput();
    UpdateState(m_TimeDelta);
    CheckCollisions(m_TimeDelta);
    UpdatePhysics(m_TimeDelta);
    UpdateUniformGrid(m_TimeDelta);
    RenderFrame();
    //m_LightingModel->MoveLights(m_TimeDelta);

    for (int i = 0; i < ballCount; i++)
    {
        balls[i]->CollisionCount = 0;
    }
    //ball->CollisionCount = 0;
    return m_QuitApp;
}

// TODO: Add MOUSE support for looking around.
// TODO: Might move all this as "core" functionality in RTRApp class.
void MainApp::CheckInput()
{
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: m_QuitApp = true; break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: m_QuitApp = true; break;
                    case SDLK_w: m_MovingForward = true; break;
                    case SDLK_s: m_MovingBackward = true; break;
                    case SDLK_a: m_StrafingLeft = true; break;
                    case SDLK_d: m_StrafingRight = true; break;
                    case SDLK_LEFT: m_TurningLeft = true; break;
                    case SDLK_RIGHT: m_TurningRight = true; break;
                    case SDLK_UP: m_TiltingUp = true; break;
                    case SDLK_DOWN: m_TiltingDown = true; break;
                    case SDLK_MINUS: m_BoardTiltDown = true; break;
                    case SDLK_EQUALS: m_BoardTiltUp = true; break;
                    case SDLK_SPACE: m_LaunchBall = true; break;
                    case SDLK_h: m_HUDDisplay = !m_HUDDisplay; break;
                    case SDLK_z: m_LeftFlipper = true; break;
                    case SDLK_x: m_RightFlipper = true; break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_w: m_MovingForward = false; break;
                    case SDLK_s: m_MovingBackward = false; break;
                    case SDLK_a: m_StrafingLeft = false; break;
                    case SDLK_d: m_StrafingRight = false; break;
                    case SDLK_LEFT: m_TurningLeft = false; break;
                    case SDLK_RIGHT: m_TurningRight = false; break;
                    case SDLK_UP: m_TiltingUp = false; break;
                    case SDLK_DOWN: m_TiltingDown = false; break;
                    case SDLK_MINUS: m_BoardTiltDown = false; break;
                    case SDLK_EQUALS: m_BoardTiltUp = false; break;
                    case SDLK_SPACE: m_LaunchBall = false; break;
                    case SDLK_z: m_LeftFlipper = false; break;
                    case SDLK_x: m_RightFlipper = false; break;
                }
                break;
        }
    }
}

void MainApp::UpdateState(unsigned int td_milli)
{
    // Update camera position based on keybard state checked earlier and saved
    if (m_MovingForward == true) m_Camera->MoveForward(td_milli);
    if (m_MovingBackward == true) m_Camera->MoveBackward(td_milli);
    if (m_TurningLeft == true) m_Camera->TurnLeft(td_milli);
    if (m_TurningRight == true) m_Camera->TurnRight(td_milli);
    if (m_StrafingLeft == true) m_Camera->StrafeLeft(td_milli);
    if (m_StrafingRight == true) m_Camera->StrafeRight(td_milli);
    if (m_TiltingUp == true) m_Camera->TiltUp(td_milli);
    if (m_TiltingDown == true) m_Camera->TiltDown(td_milli);

    if (m_BoardTiltDown == true)
    {
        if (Gravity > -1.1f)
        {
            Gravity -= (0.36f * td_milli / 1000.0f);
            table->m_Transform = glm::rotate(table->m_Transform, glm::radians(10.0f * td_milli / 1000.0f), glm::vec3(-1.0f, 0.0f, 0.0));
        }
    }
        
    if (m_BoardTiltUp == true)
    {
        if (Gravity < 1.1f)
        {
            Gravity += (0.36f * td_milli / 1000.0f);
            table->m_Transform = glm::rotate(table->m_Transform, glm::radians(10.0f * td_milli / 1000.0f), glm::vec3(1.0f, 0.0f, 0.0));
        }
    }

    if (m_LaunchBall == true)
    {
        if (power < 100.0f)
        {
            power += (50.0f * td_milli / 1000.0f);
            launch += 0.1 * td_milli / 1000.0f;
            plunger->m_Transform = glm::translate(plunger->m_Transform, glm::vec3(0.0, 0.0, 0.1 * td_milli / 1000.0f));
        }
        else if (power > 100.0f)
        {
            power = 100.0f;
        }
    }
    else if (m_LaunchBall == false)
    {
        if (power > 0.0f)
        {
            ball = new RTRSphere();
            ball->Init(0.1f);
            ball->m_Transform = glm::translate(ball->m_Transform, glm::vec3(2.85, 0.3, 4.7));
            //ball->m_Transform = glm::scale(ball->m_Transform, glm::vec3(0.1, 0.1, 0.1));
            table->m_ChildObjects.push_back(ball);
            ball->label = "Ball" + ballCount;
            ball->IsDynamic = true;
            ball->Velocity = glm::vec2(0.0f, -10.5 - power/10.0f);

            balls.push_back(ball);
            DynamicObjects.push_back(ball);

            ball->CalculateBoundingVolume(glm::mat4(1.0f));

            ballCount++;

            plunger->m_Transform = glm::translate(plunger->m_Transform, glm::vec3(0.0, 0.0, -launch));
            launch = 0.0f;
        }
         power = 0.0f;
    }

    if (m_LeftFlipper == true)
    {
        if (lFlipperAngle < 45.0f)
        {
            lFlipperAngle += FlipperRotationSpeed * td_milli / 1000.0f;
            flipperLParent->m_Transform = glm::rotate(flipperLParent->m_Transform, glm::radians(FlipperRotationSpeed * td_milli / 1000.0f), glm::vec3(0.0, 1.0, 0.0));
            flipperL->CalculateBoundingVolume(flipperLParent->m_Transform);
            flipperL->RotationSpeed = FlipperRotationSpeed / 150.0f;
        }
        else if (lFlipperAngle >= 45.0f)
        {
            flipperL->RotationSpeed = 0.0f;
        }
    }
    else if (m_LeftFlipper == false)
    {
        flipperLParent->m_Transform = glm::rotate(flipperLParent->m_Transform, glm::radians(-lFlipperAngle), glm::vec3(0.0, 1.0, 0.0));
        lFlipperAngle = 0.0f;
        flipperL->CalculateBoundingVolume(flipperLParent->m_Transform);
        flipperL->RotationSpeed = 0.0f;
    }

    if (m_RightFlipper == true)
    {
        if (rFlipperAngle > -45.0f)
        {
            rFlipperAngle -= FlipperRotationSpeed * td_milli / 1000.0f;
            flipperRParent->m_Transform = glm::rotate(flipperRParent->m_Transform, glm::radians(-FlipperRotationSpeed * td_milli / 1000.0f), glm::vec3(0.0, 1.0, 0.0));
            flipperR->CalculateBoundingVolume(flipperRParent->m_Transform);
            flipperR->RotationSpeed = FlipperRotationSpeed/150.0f;
        }
        else if (rFlipperAngle <= -45.0f)
        {
            flipperR->RotationSpeed = 0.0f;
        }
    }
    else if (m_RightFlipper == false)
    {
        flipperRParent->m_Transform = glm::rotate(flipperRParent->m_Transform, glm::radians(-rFlipperAngle), glm::vec3(0.0, 1.0, 0.0));
        rFlipperAngle = 0.0f;
        flipperR->CalculateBoundingVolume(flipperRParent->m_Transform);
        flipperR->RotationSpeed = 0.0f;
    }

    // Update directional camera to align with camera forward direction
    m_LightingModel->GetLight(0)->Direction = m_Camera->m_Front;

    // Setup Model and View matrices
    m_ModelMatrix = glm::mat4(1.0f);
    m_ViewMatrix = m_Camera->GetViewMatrix();
}

void MainApp::UpdatePhysics(unsigned int td_milli)
{
    float TimeInMS = (float)td_milli / 1000.0f;

    for (int i = 0; i < ballCount; i++)
    {
        //Update gravity
        balls[i]->Velocity.y += TimeInMS * (Gravity / std::sin(TableAngle));

        //Add friction
        balls[i]->Velocity = balls[i]->Velocity * (1.0f - (TimeInMS * 0.3f));

        //Move ball by velocity
        glm::vec3 TranslationVector = glm::vec3(balls[i]->Velocity.x, 0.0f, balls[i]->Velocity.y) * TimeInMS;
        balls[i]->m_Transform = glm::translate(balls[i]->m_Transform, TranslationVector);

        balls[i]->CalculateBoundingVolume(glm::mat4(1.0f));
    }

    bumper->CentreOfRotation = glm::vec2(1.45, -0.5);
    bumper->m_Transform = glm::rotate(bumper->m_Transform, glm::radians(50.0f) * TimeInMS, glm::vec3(0.0, 1.0, 0.0));
    bumper->CalculateBoundingVolume(glm::mat4(1.0f));
    bumper->RotationSpeed = glm::radians(50.0f);

    bumperCircle->movement += TimeInMS;
    if (bumperCircle->movement >= 7)
    {
    bumperCircle->movement = -7;
    }
    else if (bumperCircle->movement < 0)
    {
        bumperCircle->m_Transform = glm::translate(bumperCircle->m_Transform, glm::vec3(0.0, 0.0, -0.5 * TimeInMS));
    }
    else if (bumperCircle->movement > 0)
    {
        bumperCircle->m_Transform = glm::translate(bumperCircle->m_Transform, glm::vec3(0.0, 0.0, 0.5 * TimeInMS));
    }

    bumperCircle->CalculateBoundingVolume(glm::mat4(1.0f));
}

glm::vec2 MainApp::ProjectVectorOntoAxis(glm::vec2 Vector, glm::vec2 Axis)
{
   return (glm::dot(Vector, Axis)) / std::pow(glm::length(Axis), 2.0f) * Axis;
}

void MainApp::UpdateUniformGrid(unsigned int td_milli)
{
    for (int i = 0; i < UniformGrid.size(); i++)
    {
        for (int j = UniformGrid[i].OverlappingObjects.size() - 1; j >= 0; j--)
        {
            //Remove any dynamic objects before checking them again
            if (UniformGrid[i].OverlappingObjects[j]->IsDynamic)
            {
                UniformGrid[i].OverlappingObjects.erase(UniformGrid[i].OverlappingObjects.begin() + j);
            }
        }

        for (RTRObject* obj : DynamicObjects)
        {
            if (obj->OverlapsInterval(UniformGrid[i].MinimumPoint, UniformGrid[i].MaximumPoint))
            {
                UniformGrid[i].OverlappingObjects.push_back(obj);
            }
        }
    }
}

void MainApp::CheckCollisions(unsigned int td_milli)
{
    for (UniformGridCell cell : UniformGrid)
    {
        if (cell.OverlappingObjects.size() > 1)
        {
            for (int i = 0; i < cell.OverlappingObjects.size() - 1; i++)
            {
                for (int j = i + 1; j < cell.OverlappingObjects.size(); j++)
                {
                    if (cell.OverlappingObjects[i]->IsColliding(cell.OverlappingObjects[j]))
                    {
                        RTRSphere* Ball = nullptr;
                        if (cell.OverlappingObjects[i]->GetName() == "RTRSphere")
                        {
                            Ball = (RTRSphere*)cell.OverlappingObjects[i];
                            cell.OverlappingObjects[j]->CheckForCollision(Ball);
                        }
                        else if (cell.OverlappingObjects[j]->GetName() == "RTRSphere")
                        {
                            Ball = (RTRSphere*)cell.OverlappingObjects[j];
                            cell.OverlappingObjects[i]->CheckForCollision(Ball);
                        }
                    }
                }
            }
        }
    }
}

void MainApp::RenderFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the shaded cube using the default blinn-phong shader
    glUseProgram(m_DefaultShader->GetId());
    m_DefaultShader->SetFloat("u_CurTime", (float)m_CurTime);
    m_DefaultShader->SetFloat("u_TimeDelta", (float)m_TimeDelta);
    //m_DefaultShader->SetMat4("u_ModelMatrix", m_ModelMatrix);
    m_DefaultShader->SetMat4("u_ViewMatrix", m_ViewMatrix);
    m_DefaultShader->SetMat4("u_ProjectionMatrix", m_ProjectionMatrix);
    m_DefaultShader->SetCamera("u_Camera", *m_Camera);
    m_DefaultShader->SetLightingModel(*m_LightingModel);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(m_DefaultShader->GetId(), "ourTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(m_DefaultShader->GetId(), "ourSpecular"), 0);

    glm::mat4 identityMatrix = glm::mat4(1.0);
    DrawObject(table, identityMatrix, m_DefaultShader);

    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
    //glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);
    
    // Print out all debug info
    if (m_HUDDisplay)
    {
        m_Console->DebugRender("DEBUG", m_FPS,
            m_Camera->m_Position.x, m_Camera->m_Position.y, m_Camera->m_Position.z,
            m_Camera->m_Yaw, m_Camera->m_Pitch, Gravity, power);
    }
    else
    {
        m_Console->Render("EXTREME PINBALL", m_FPS,
            m_Camera->m_Position.x, m_Camera->m_Position.y, m_Camera->m_Position.z,
            m_Camera->m_Yaw, m_Camera->m_Pitch, Gravity, power);
    }


    // Swap buffers
    SDL_GL_SwapWindow(m_SDLWindow);
}

void MainApp::InitialiseUniformGrid()
{
    glm::vec2 StartingPoint = glm::vec2(-ColumnsOfCells / 2 * IntervalSize, -RowsOfCells / 2 * IntervalSize);
    glm::vec2 SizeOfCells = glm::vec2(IntervalSize, IntervalSize);

    std::vector<RTRObject*> AllObjects = table->GetAllChildren();

    for (int x = 0; x < ColumnsOfCells; x++)
    {
        for (int y = 0; y < RowsOfCells; y++)
        {
            glm::vec2 CurrentMinPoint = StartingPoint + glm::vec2(x * IntervalSize, y * IntervalSize);
            glm::vec2 CurrentMaxPoint = CurrentMinPoint + SizeOfCells;

            UniformGridCell CurrentCell;
            CurrentCell.MinimumPoint = CurrentMinPoint;
            CurrentCell.MaximumPoint = CurrentMaxPoint;

            for (RTRObject* obj : AllObjects)
            {
                if (!obj->IgnoreCollision)
                {
                    if (obj->OverlapsInterval(CurrentMinPoint, CurrentMaxPoint))
                    {
                        CurrentCell.OverlappingObjects.push_back(obj);
                    }
                }
            }

            UniformGrid.push_back(CurrentCell);
        }
    }
}

//-----------------------------------------------------------------------------
static const char* MAIN_WINDOW_TITLE = "RMIT COSC1226: Real-Time Rendering";
int main(int argc, char** argv)
{
    MainApp* app = new MainApp(MAIN_WINDOW_TITLE, true, 1440, 810);
    if (int err = app->Init() != 0) {
        std::cout << "RTR:ERROR: Failed to initialise application. Please see error messages for details..." << std::endl;
        return -1;
    };
    app->Run();
    app->Done();
    return 0;
}

void MainApp::DrawObject(RTRObject* object, glm::mat4 parentTransform, RTRShader* shader)
{
    glm::mat4 objectTransform = parentTransform * object->m_Transform;
    
    shader->SetMat4("u_ModelMatrix", objectTransform);

    object->Render(shader);

    for (RTRObject* child : object->m_ChildObjects)
    {
        if (child && child->GetName() == "RTRCube")
        {
            glUniform1i(glGetUniformLocation(m_DefaultShader->GetId(), "isTextured"), 1);
            DrawObject(child, objectTransform, shader);
        }
        else if (child)
        {
            glUniform1i(glGetUniformLocation(m_DefaultShader->GetId(), "isTextured"), 0);
            DrawObject(child, objectTransform, shader);
        }
    }
}
