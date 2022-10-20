import shutil
import os
import re  
assignment = """<?xml version="1.0" ?>
<argos-configuration>
  <framework>
    <system threads="0" />
    <experiment length="0" ticks_per_second="10" random_seed="124" />
  </framework>
  <controllers>
    <assignment2_{controller}_controller id="fdc" library="build/controllers/assignment2_{controller}/libassignment2_{controller}">
      <actuators>
        <differential_steering implementation="default" />
      </actuators>
      <sensors>
        <footbot_proximity implementation="default" show_rays="true" />
        <positioning implementation="default"/>
      </sensors>
      <params alpha="7.5" delta="0.1" velocity="5" />
    </assignment2_{controller}_controller>
  </controllers>
  <arena size="5, 5, 1" center="0,0,0.5">
    <box id="wall_north" size="5,0.1,0.5" movable="false">
      <body position="0,2.5,0" orientation="0,0,0" />
    </box>
    <box id="wall_south" size="5,0.1,0.5" movable="false">
      <body position="0,-2.5,0" orientation="0,0,0" />
    </box>
    <box id="wall_east" size="0.1,5,0.5" movable="false">
      <body position="2.5,0,0" orientation="0,0,0" />
    </box>
    <box id="wall_west" size="0.1,5,0.5" movable="false">
      <body position="-2.5,0,0" orientation="0,0,0" />
    </box>
    <foot-bot id="fb_0">
      <body position="0,0,0" orientation="0,0,0" />
      <controller config="fdc"/>
    </foot-bot>
  </arena>
  <physics_engines>
    <dynamics2d id="dyn2d" />
  </physics_engines>
  <media />
  <visualization>
    <qt-opengl>
      <camera>
        <placements>
          <placement index="0" position="0,0,8.14689" look_at="0,0,0" up="1,0,0" lens_focal_length="65" />
        </placements>
      </camera>
    </qt-opengl>
  </visualization>
</argos-configuration>"""
sublist ="""add_library(assignment2_{controller} MODULE assignment2.h assignment2_{controller}.cpp)
target_link_libraries(assignment2_{controller}
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot)
"""
shutil.rmtree("./experiments")
shutil.rmtree("./controllers")
os.makedirs("./experiments", exist_ok=True)
os.makedirs("./controllers", exist_ok=True)
makeline = 'add_subdirectory(assignment2_{controller})\n'
shutil.copyfile("CMakeLists.txt", "./controllers/CMakeLists.txt")
makelist = open("./controllers/CMakeLists.txt", "a")
for filename in os.scandir('./submissions'):
    if ".cpp" in filename.path:
        controller=filename.name[:-4]
        os.makedirs('./controllers/assignment2_{controller}'.format(controller=controller), exist_ok=True)
        shutil.copyfile(filename.path, './controllers/assignment2_{controller}/assignment2_{controller}.cpp'.format(controller=controller))
        cpp = open("./controllers/assignment2_{controller}/assignment2_{controller}.cpp".format(controller=controller), "r")
        data = cpp.read()
        cpp.close()
        cpp = open("./controllers/assignment2_{controller}/assignment2_{controller}.cpp".format(controller=controller), "w")
        cpp.write(re.sub(r'REGISTER_CONTROLLER\(CFootBotDiffusion, \"assignment[0-9]_controller\"\)','REGISTER_CONTROLLER(CFootBotDiffusion, "assignment2_{controller}_controller")'.format(controller=controller),data))
        cpp.close()
        shutil.copyfile('./assignment2.h', './controllers/assignment2_{controller}/assignment2.h'.format(controller=controller))
        (open("./controllers/assignment2_{controller}/CMakeLists.txt".format(controller=controller), "a").write(sublist.format(controller=controller)))
        makelist.write(makeline.format(controller=controller))
        (open("./experiments/assignment2_{controller}.argos".format(controller=controller), "a").write(assignment.format(controller=controller)))
makelist.close()