#include "Spider.h"

ConfigableGroupModel* LegCalf(){
    ConfigableGroupModel* group = new ConfigableGroupModel({
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_TETRAHEDRON)

    }, {
        {"CALF_LENGTH", 0.24},
        {"calf_width", 0.05},
        {"calf_depth", 0.05},
        {"calf_indent", 0.01}
    }, [](std::vector<Model*>& models, std::map<std::string, float>& params) {
        float CALF_LENGTH = params["CALF_LENGTH"];
        float calf_width = params["calf_width"];
        float calf_depth = params["calf_depth"];
        float calf_indent = params["calf_indent"];

        float _current_dist = CALF_LENGTH;
        float _size =  (calf_width + calf_depth)/2;
        Model* cone = models[3];
        cone->setAnglesDegrees(glm::vec3(0,-90,0));
        cone->setScale(glm::vec3(calf_width,_size,calf_depth));
        cone->setPosition(glm::vec3(0,0,_current_dist - _size/2));
        _current_dist -= _size;

        _size = calf_width/2;
        Model* cube_leg = models[2];
        cube_leg->setAnglesDegrees(glm::vec3(0,-90,0));
        cube_leg->setScale(glm::vec3(calf_width,_size,calf_depth));
        cube_leg->setPosition(glm::vec3(0,0,_current_dist - _size/2));
        _current_dist -= _size;
        
        Model* cube_calf_thin = models[1];
        _size = _current_dist/2;
        cube_calf_thin->setAnglesDegrees(glm::vec3(0,-90,0));
        cube_calf_thin->setScale(glm::vec3(calf_indent, _size ,calf_depth*0.8));
        cube_calf_thin->setPosition(glm::vec3(0,0,_current_dist - _size/2));
        _current_dist -= _size;

        Model* cube_top_calf = models[0];
        _size = _current_dist;
        cube_top_calf->setAnglesDegrees(glm::vec3(0,-90,0));
        cube_top_calf->setScale(glm::vec3(calf_width, _size ,calf_depth));
        cube_top_calf->setPosition(glm::vec3(0,0,_current_dist - _size/2));


        //set colors
        for(Model* model : models) {
            model->setColor(glm::vec4(0.4,0.4,0.6,1));
        }

    });
    group->updateModels();

    return group;
}

ConfigableGroupModel* LegHip(){
    ConfigableGroupModel* group = new ConfigableGroupModel({
        new Primitive(PRIM_CYLINDER)
    }, {
        {"HIP_LENGTH", 0.04},
        {"hip_width", 0.02}
    }, [](std::vector<Model*>& models, std::map<std::string, float>& params) {
        float HIP_LENGTH = params["HIP_LENGTH"];
        float hip_width = params["hip_width"];

        Model* cylinder = models[0];
        cylinder->setAnglesDegrees(glm::vec3(0,-90,0));
        cylinder->setScale(glm::vec3(hip_width,HIP_LENGTH,hip_width));
        cylinder->setPosition(glm::vec3(0,0,HIP_LENGTH/2));

        cylinder->setColor(glm::vec4(0.4,0.4,0.6,1));

    });

    return group;
}

ConfigableGroupModel* Leg(){
    ConfigableGroupModel* group = new ConfigableGroupModel({
        LegHip(),
        LegThigh(),
        LegCalf()
    }, {
        {"TIBIA_THETA", 0},
        {"FEMUR_THETA", 0},
        {"COXA_THETA", 0},
        {"HIP_LENGTH", 0.04},
        {"hip_width", 0.02},
        {"THIGH_LENGTH", 0.2},
        {"thigh_width", 0.04},
        {"thigh_depth", 0.05},
        {"thigh_indent", 0.02},
        {"CALF_LENGTH", 0.24},
        {"calf_width", 0.05},
        {"calf_depth", 0.05},
        {"calf_indent", 0.01}
    }, [](std::vector<Model*>& models, std::map<std::string, float>& params) {
        float TIBIA_THETA = params["TIBIA_THETA"];
        float FEMUR_THETA = params["FEMUR_THETA"];
        float COXA_THETA = params["COXA_THETA"];
        float HIP_LENGTH = params["HIP_LENGTH"];
        float hip_width = params["hip_width"];
        float THIGH_LENGTH = params["THIGH_LENGTH"];
        float thigh_width = params["thigh_width"];
        float thigh_depth = params["thigh_depth"];
        float thigh_indent = params["thigh_indent"];
        float CALF_LENGTH = params["CALF_LENGTH"];
        float calf_width = params["calf_width"];
        float calf_depth = params["calf_depth"];
        float calf_indent = params["calf_indent"];

        float _current_dist = 0;
        ConfigableGroupModel* hip = dynamic_cast<ConfigableGroupModel*>(models[0]);
        hip->setPosition(glm::vec3(0,0,_current_dist));
        hip->setAngles(glm::vec3(COXA_THETA,0,0));
        _current_dist += HIP_LENGTH;

        hip->setFloatParams({
            {"HIP_LENGTH", HIP_LENGTH},
            {"hip_width", hip_width}
        });

        ConfigableGroupModel* thigh = dynamic_cast<ConfigableGroupModel*>(models[1]);
        glm::vec4 pos = hip->getFullTransformation()*glm::vec4(0,0,HIP_LENGTH,1);
        thigh->setPosition(glm::vec3(pos.x,pos.y,pos.z));
        thigh->setAngles(glm::vec3(COXA_THETA,-FEMUR_THETA,0));
        _current_dist += THIGH_LENGTH;

        thigh->setFloatParams({
            {"THIGH_LENGTH", THIGH_LENGTH},
            {"thigh_width", thigh_width},
            {"thigh_depth", thigh_depth},
            {"thigh_indent", thigh_indent}
        });

        ConfigableGroupModel* calf = dynamic_cast<ConfigableGroupModel*>(models[2]);
        pos = thigh->getFullTransformation()*glm::vec4(0,0,THIGH_LENGTH,1);
        calf->setPosition(glm::vec3(pos.x,pos.y,pos.z));
        calf->setAngles(glm::vec3(COXA_THETA,-FEMUR_THETA-TIBIA_THETA,0));

        calf->setFloatParams({
            {"CALF_LENGTH", CALF_LENGTH},
            {"calf_width", calf_width},
            {"calf_depth", calf_depth},
            {"calf_indent", calf_indent}
        });

    });
    group->updateModels();

    return group;
}

ConfigableGroupModel* LegThigh(){
    ConfigableGroupModel* group = new ConfigableGroupModel({
        
        new Primitive(PRIM_CYLINDER),
        new Primitive(PRIM_CYLINDER),
        new Primitive(PRIM_CUBE),
        motor()

    }, {
        {"THIGH_LENGTH", 0.2},
        {"thigh_width", 0.04},
        {"thigh_depth", 0.05},
        {"thigh_indent", 0.02}
    }, [](std::vector<Model*>& models, std::map<std::string, float>& params) {
        float THIGH_LENGTH = params["THIGH_LENGTH"];
        float thigh_width = params["thigh_width"];
        float thigh_depth = params["thigh_depth"];
        float thigh_indent = params["thigh_indent"];

        float _current_dist = THIGH_LENGTH;
        Model* motor = models[3];
        motor->setAnglesDegrees(glm::vec3(0,0,90));
        motor->setPosition(glm::vec3(0.01,0,_current_dist));
        _current_dist -= 0.01;

        float _size = thigh_width/2;
        Model* cube_leg = models[2];
        cube_leg->setAnglesDegrees(glm::vec3(0,-90,0));
        cube_leg->setScale(glm::vec3(thigh_width,_size,thigh_depth));
        cube_leg->setPosition(glm::vec3(0,0,_current_dist - _size/2));
        _current_dist -= _size;
        
        Model* cube_thigh_thin = models[1];
        _size = _current_dist/2;
        cube_thigh_thin->setAnglesDegrees(glm::vec3(0,-90,0));
        cube_thigh_thin->setScale(glm::vec3(thigh_indent, _size ,thigh_depth*0.8));
        cube_thigh_thin->setPosition(glm::vec3(0,0,_current_dist - _size/2));
        _current_dist -= _size;

        Model* cube_top_thigh = models[0];
        _size = _current_dist;
        cube_top_thigh->setAnglesDegrees(glm::vec3(0,-90,0));
        cube_top_thigh->setScale(glm::vec3(thigh_width, _size ,thigh_depth));
        cube_top_thigh->setPosition(glm::vec3(0,0,_current_dist - _size/2));


        //set colors
        for(Model* model : models) {
            model->setColor(glm::vec4(0.4,0.4,0.6,1));
        }

    });
    group->updateModels();

    return group;
}

Model* motor(){
    ObjModel* obj = new ObjModel("resources\\motor.obj","resources\\motor.png");
    // obj->setScale(glm::vec3(0.001, 0.001, 0.001));
    return obj;
}
