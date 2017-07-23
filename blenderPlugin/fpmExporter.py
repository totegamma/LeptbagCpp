import bpy
from bpy_extras.io_utils import ExportHelper

bl_info = {
    "name": "Friends Physical Model Exporter",
    "category": "Import-Export",
}

class fpmExporter(bpy.types.Operator, ExportHelper):
    bl_label = 'export fpm'
    bl_idname = 'export.fpm'
    filename_ext = '.fpm' # ←必須

    def execute(self, context):

        fo = open(self.filepath, 'w')


        fo.write("[\n")

        for keyname in bpy.data.objects.keys():
            obj = bpy.data.objects[keyname]
            if obj.type == "EMPTY":
                fo.write("\t{\n")
                fo.write("\t\t\"objectType\":\"constraint\",\n")
                fo.write("\t\t\"name\":\"%s\",\n" % keyname)
                fo.write("\t\t\"constraintType\":\"%s\",\n" % obj.rigid_body_constraint.type)
                fo.write("\t\t\"xpos\":%f,\n" % obj.location[1])
                fo.write("\t\t\"ypos\":%f,\n" % obj.location[2])
                fo.write("\t\t\"zpos\":%f,\n" % obj.location[0])
                fo.write("\t\t\"xrot\":%f,\n" % obj.rotation_euler[1])
                fo.write("\t\t\"yrot\":%f,\n" % obj.rotation_euler[2])
                fo.write("\t\t\"zrot\":%f,\n" % obj.rotation_euler[0])
                fo.write("\t\t\"object1\":\"%s\",\n" % obj.rigid_body_constraint.object1.name)
                fo.write("\t\t\"object1xpos\":%f,\n" % (obj.location[1] - obj.rigid_body_constraint.object1.location[1]))
                fo.write("\t\t\"object1ypos\":%f,\n" % (obj.location[2] - obj.rigid_body_constraint.object1.location[2]))
                fo.write("\t\t\"object1zpos\":%f,\n" % (obj.location[0] - obj.rigid_body_constraint.object1.location[0]))
                fo.write("\t\t\"object2\":\"%s\",\n" % obj.rigid_body_constraint.object2.name)
                fo.write("\t\t\"object2xpos\":%f,\n" % (obj.location[1] - obj.rigid_body_constraint.object2.location[1]))
                fo.write("\t\t\"object2ypos\":%f,\n" % (obj.location[2] - obj.rigid_body_constraint.object2.location[2]))
                fo.write("\t\t\"object2zpos\":%f,\n" % (obj.location[0] - obj.rigid_body_constraint.object2.location[0]))
                fo.write("\t\t\"useLimit\":\"%s\",\n" % obj.rigid_body_constraint.use_limit_ang_z)
                fo.write("\t\t\"limitLower\":%f,\n" % obj.rigid_body_constraint.limit_ang_z_lower)
                fo.write("\t\t\"limitUpper\":%f\n" % obj.rigid_body_constraint.limit_ang_z_upper)
                fo.write("\t},\n")
            elif obj.type == "MESH":

                if obj.rigid_body:
                    fo.write("\t{\n")
                    fo.write("\t\t\"objectType\":\"MESH\",\n")
                    fo.write("\t\t\"name\":\"%s\",\n" % keyname)
                    fo.write("\t\t\"rigidBodyType\":\"%s\",\n" % obj.rigid_body.type)
                    fo.write("\t\t\"rigidBodyShape\":\"%s\",\n" % obj.rigid_body.collision_shape)
                    fo.write("\t\t\"mass\":%f,\n" % obj.rigid_body.mass)
                    fo.write("\t\t\"friction\":%f,\n" % obj.rigid_body.friction)
                    fo.write("\t\t\"restitution\":%f,\n" % obj.rigid_body.restitution)
                    fo.write("\t\t\"xpos\":%f,\n" % obj.location[1])
                    fo.write("\t\t\"ypos\":%f,\n" % obj.location[2])
                    fo.write("\t\t\"zpos\":%f,\n" % obj.location[0])
                    fo.write("\t\t\"xrot\":%f,\n" % obj.rotation_euler[1])
                    fo.write("\t\t\"yrot\":%f,\n" % obj.rotation_euler[2])
                    fo.write("\t\t\"zrot\":%f,\n" % obj.rotation_euler[0])
                    fo.write("\t\t\"wqat\":%f,\n" % obj.rotation_quaternion[0])
                    fo.write("\t\t\"xqat\":%f,\n" % obj.rotation_quaternion[2])
                    fo.write("\t\t\"yqat\":%f,\n" % obj.rotation_quaternion[3])
                    fo.write("\t\t\"zqat\":%f,\n" % obj.rotation_quaternion[1])
                    fo.write("\t\t\"xscl\":%f,\n" % obj.scale[1])
                    fo.write("\t\t\"yscl\":%f,\n" % obj.scale[2])
                    fo.write("\t\t\"zscl\":%f,\n" % obj.scale[0])

                    if obj.rigid_body.collision_shape == "CONVEX_HULL":
                        fo.write("\t\t\"vertex\": [\n")

                        data = obj.data
                        polys = data.polygons
                        verts = data.vertices

                        colorList = []
                        cordList  = []
                        normList  = []

                        for material in data.materials:
                            colorList.append(material.diffuse_color)

                        for vert in verts:
                            cordList.append(vert.co)
                            normList.append(vert.normal)



                        isFirst = True
                        for poly in polys:
                            if isFirst == True:
                                isFirst = False;
                            else:
                                fo.write(",\n\n")

                            fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f],\n" % (
                                                                            cordList[poly.vertices[0]][1],
                                                                            cordList[poly.vertices[0]][2],
                                                                            cordList[poly.vertices[0]][0],
                                                                            normList[poly.vertices[0]][1],
                                                                            normList[poly.vertices[0]][2],
                                                                            normList[poly.vertices[0]][0],
                                                                            colorList[poly.material_index][0],
                                                                            colorList[poly.material_index][1],
                                                                            colorList[poly.material_index][2]))

                            fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f],\n" % (
                                                                            cordList[poly.vertices[1]][1],
                                                                            cordList[poly.vertices[1]][2],
                                                                            cordList[poly.vertices[1]][0],
                                                                            normList[poly.vertices[1]][1],
                                                                            normList[poly.vertices[1]][2],
                                                                            normList[poly.vertices[1]][0],
                                                                            colorList[poly.material_index][0],
                                                                            colorList[poly.material_index][1],
                                                                            colorList[poly.material_index][2]))

                            fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f]" % (
                                                                            cordList[poly.vertices[2]][1],
                                                                            cordList[poly.vertices[2]][2],
                                                                            cordList[poly.vertices[2]][0],
                                                                            normList[poly.vertices[2]][1],
                                                                            normList[poly.vertices[2]][2],
                                                                            normList[poly.vertices[2]][0],
                                                                            colorList[poly.material_index][0],
                                                                            colorList[poly.material_index][1],
                                                                            colorList[poly.material_index][2]))

                        fo.write("\n")

                        fo.write("\t\t]\n")


                    fo.write("\t},\n")

                else:

                    fo.write("\t{\n")
                    fo.write("\t\t\"objectType\":\"MESH\",\n")
                    fo.write("\t\t\"name\":\"%s\",\n" % keyname)
                    fo.write("\t\t\"xpos\":%f,\n" % obj.location[1])
                    fo.write("\t\t\"ypos\":%f,\n" % obj.location[2])
                    fo.write("\t\t\"zpos\":%f,\n" % obj.location[0])
                    fo.write("\t\t\"xrot\":%f,\n" % obj.rotation_euler[1])
                    fo.write("\t\t\"yrot\":%f,\n" % obj.rotation_euler[2])
                    fo.write("\t\t\"zrot\":%f,\n" % obj.rotation_euler[0])
                    fo.write("\t\t\"wqat\":%f,\n" % obj.rotation_quaternion[0])
                    fo.write("\t\t\"xqat\":%f,\n" % obj.rotation_quaternion[2])
                    fo.write("\t\t\"yqat\":%f,\n" % obj.rotation_quaternion[3])
                    fo.write("\t\t\"zqat\":%f,\n" % obj.rotation_quaternion[1])
                    fo.write("\t\t\"xscl\":%f,\n" % obj.scale[1])
                    fo.write("\t\t\"yscl\":%f,\n" % obj.scale[2])
                    fo.write("\t\t\"zscl\":%f,\n" % obj.scale[0])


                    fo.write("\t\t\"vertex\": [\n")

                    data = obj.data
                    polys = data.polygons
                    verts = data.vertices

                    colorList = []
                    cordList  = []
                    normList  = []

                    for material in data.materials:
                        colorList.append(material.diffuse_color)

                    for vert in verts:
                        cordList.append(vert.co)
                        normList.append(vert.normal)



                    isFirst = True
                    for poly in polys:
                        if isFirst == True:
                            isFirst = False;
                        else:
                            fo.write(",\n\n")

                        fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f],\n" % (
                                                                        cordList[poly.vertices[0]][1],
                                                                        cordList[poly.vertices[0]][2],
                                                                        cordList[poly.vertices[0]][0],
                                                                        normList[poly.vertices[0]][1],
                                                                        normList[poly.vertices[0]][2],
                                                                        normList[poly.vertices[0]][0],
                                                                        colorList[poly.material_index][0],
                                                                        colorList[poly.material_index][1],
                                                                        colorList[poly.material_index][2]))

                        fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f],\n" % (
                                                                        cordList[poly.vertices[1]][1],
                                                                        cordList[poly.vertices[1]][2],
                                                                        cordList[poly.vertices[1]][0],
                                                                        normList[poly.vertices[1]][1],
                                                                        normList[poly.vertices[1]][2],
                                                                        normList[poly.vertices[1]][0],
                                                                        colorList[poly.material_index][0],
                                                                        colorList[poly.material_index][1],
                                                                        colorList[poly.material_index][2]))

                        fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f]" % (
                                                                        cordList[poly.vertices[2]][1],
                                                                        cordList[poly.vertices[2]][2],
                                                                        cordList[poly.vertices[2]][0],
                                                                        normList[poly.vertices[2]][1],
                                                                        normList[poly.vertices[2]][2],
                                                                        normList[poly.vertices[2]][0],
                                                                        colorList[poly.material_index][0],
                                                                        colorList[poly.material_index][1],
                                                                        colorList[poly.material_index][2]))

                    fo.write("\n")

                    fo.write("\t\t]\n")

                    fo.write("\t},\n")



        fo.write("\t{\n")
        fo.write("\t\t\"objectType\":\"DUMMY\"\n")
        fo.write("\t}\n")

        fo.write("]\n")


        fo.close()


        return {'FINISHED'}


def menu_func(self, context):
    self.layout.operator(fpmExporter.bl_idname, text="Friends Physical Model (.fpm)")

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)
