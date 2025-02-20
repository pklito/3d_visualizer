#include "Spider.h"

ConfigableGroupModel* LegCalf(){
    ConfigableGroupModel* group = new ConfigableGroupModel({
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_CUBE),
        new Primitive(PRIM_TETRAHEDRON)

    }, {
        {"CALF_LENGTH", 0.3},
        {"calf_width", 0.08},
        {"calf_depth", 0.1},
        {"calf_indent", 0.04}
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

ConfigableGroupModel* LegThigh(){
    return LegCalf();
}