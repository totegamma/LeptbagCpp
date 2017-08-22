import numpy as np
import bpy
from bpy_extras.io_utils import ExportHelper

bl_info = {
    "name": "Friends Physical Model Exporter",
    "category": "Import-Export",
    }

def quaternion( position, quat ):

    positionW = 1.0;
    Quat = quat[1:4]
    QuatW = quat[0]
    QuatInv = -1.0*Quat
    QuatInvW = QuatW

    #上記クォータニオンを適用
    pos = positionW*Quat + QuatW*position + np.cross( position, Quat )
    positionW = positionW*QuatW - np.dot( position, Quat )
    position = positionW*QuatInv + QuatInvW*pos + np.cross( QuatInv, pos )

    return position

def getQuatInv( quat ):

    quat[1:4] = -1.0 * quat[1:4]
    return quat

def blenderToBullet3( vec3 ):
    return np.array([ vec3[0], vec3[2], -1.0*vec3[1] ])

def blenderToBullet4( vec4 ):
    return np.array([ vec4[0], vec4[1], vec4[3], -1.0*vec4[2] ])

def bulletToBlender3( vec3 ):
    return np.array([ vec3[0], -1.0*vec3[2], vec3[1] ])

def bulletToBlender4( vec4 ):
    return np.array([ vec4[0], vec4[1], -1.0*vec4[3], vec4[2] ])



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

                hingeLocation = np.array( [ obj.location[0], obj.location[2], -1.0*obj.location[1] ], dtype=float)
                hingeQuat = np.array([-1.0 * obj.rotation_quaternion[0], obj.rotation_quaternion[1], obj.rotation_quaternion[3], -1.0*obj.rotation_quaternion[2]], dtype=float)

                obj1Location = np.array([ obj.rigid_body_constraint.object1.location[0], obj.rigid_body_constraint.object1.location[2], -1.0*obj.rigid_body_constraint.object1.location[1] ], dtype=float)
                obj1Location = hingeLocation - obj1Location

                obj2Location = np.array([ obj.rigid_body_constraint.object2.location[0], obj.rigid_body_constraint.object2.location[2], -1.0*obj.rigid_body_constraint.object2.location[1] ], dtype=float)
                obj2Location = hingeLocation - obj2Location

                hingeAxis = np.array([ 0.0, 1.0, 0.0 ], dtype=float)
                hingeAxis = quaternion( hingeAxis, hingeQuat )

                fo.write("\t{\n")
                fo.write("\t\t\"objectType\":\"constraint\",\n")
                fo.write("\t\t\"name\":\"%s\",\n" % keyname)
                fo.write("\t\t\"constraintType\":\"%s\",\n" % obj.rigid_body_constraint.type)
                fo.write("\t\t\"enabled\":\"%s\",\n" % obj.rigid_body_constraint.enabled)
                fo.write("\t\t\"xpos\":%f,\n" % hingeLocation[0])
                fo.write("\t\t\"ypos\":%f,\n" % hingeLocation[2])
                fo.write("\t\t\"zpos\":%f,\n" % hingeLocation[1])

                fo.write("\t\t\"xaxs\":%f,\n" % hingeAxis[0])
                fo.write("\t\t\"yaxs\":%f,\n" % hingeAxis[1])
                fo.write("\t\t\"zaxs\":%f,\n" % hingeAxis[2])


                fo.write("\t\t\"object1\":\"%s\",\n" % obj.rigid_body_constraint.object1.name)
                fo.write("\t\t\"object1xpos\":%f,\n" % obj1Location[0])
                fo.write("\t\t\"object1ypos\":%f,\n" % obj1Location[1])
                fo.write("\t\t\"object1zpos\":%f,\n" % obj1Location[2])

                fo.write("\t\t\"object2\":\"%s\",\n" % obj.rigid_body_constraint.object2.name)
                fo.write("\t\t\"object2xpos\":%f,\n" % obj2Location[0])
                fo.write("\t\t\"object2ypos\":%f,\n" % obj2Location[1])
                fo.write("\t\t\"object2zpos\":%f,\n" % obj2Location[2])

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

                    objectLocation = np.array([ obj.location[0], obj.location[1], obj.location[2] ], dtype=float)
                    objectQuat = np.array([ obj.rotation_quaternion[0], obj.rotation_quaternion[1], obj.rotation_quaternion[2], obj.rotation_quaternion[3] ], dtype=float)
                    objectScale = np.array([ obj.scale[0], obj.scale[1], obj.scale[2] ], dtype=float)

                    objectLocation = blenderToBullet3( objectLocation )

                    fo.write("\t\t\"xpos\":%f,\n" % objectLocation[0])
                    fo.write("\t\t\"ypos\":%f,\n" % objectLocation[1])
                    fo.write("\t\t\"zpos\":%f,\n" % (objectLocation[2]))
                    fo.write("\t\t\"wqat\":%f,\n" % 1.0)
                    fo.write("\t\t\"xqat\":%f,\n" % 0.0)
                    fo.write("\t\t\"yqat\":%f,\n" % 0.0)
                    fo.write("\t\t\"zqat\":%f,\n" % 0.0)
                    fo.write("\t\t\"xscl\":%f,\n" % 1.0)
                    fo.write("\t\t\"yscl\":%f,\n" % 1.0)
                    fo.write("\t\t\"zscl\":%f,\n" % 1.0)

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


                            absoluteVertices1 = obj.matrix_world * cordList[poly.vertices[0]] - obj.location
                            absoluteVertices2 = obj.matrix_world * cordList[poly.vertices[1]] - obj.location
                            absoluteVertices3 = obj.matrix_world * cordList[poly.vertices[2]] - obj.location

                            cordVertices = np.array([[  absoluteVertices1[0],
                                                        absoluteVertices1[1],
                                                        absoluteVertices1[2] ],
                                                    [   absoluteVertices2[0],
                                                        absoluteVertices2[1],
                                                        absoluteVertices2[2] ],
                                                    [   absoluteVertices3[0],
                                                        absoluteVertices3[1],
                                                        absoluteVertices3[2] ]])

                            cordVertices[0] = blenderToBullet3( cordVertices[0] )
                            cordVertices[1] = blenderToBullet3( cordVertices[1] )
                            cordVertices[2] = blenderToBullet3( cordVertices[2] )


                            absoluteNormals1 = obj.rotation_euler.to_matrix() * normList[poly.vertices[0]]
                            absoluteNormals2 = obj.rotation_euler.to_matrix() * normList[poly.vertices[1]]
                            absoluteNormals3 = obj.rotation_euler.to_matrix() * normList[poly.vertices[2]]

                            normVertices = np.array([[  absoluteNormals1[0],
                                                        absoluteNormals1[1],
                                                        absoluteNormals1[2] ],
                                                     [  absoluteNormals2[0],
                                                        absoluteNormals2[1],
                                                        absoluteNormals2[2] ],
                                                     [  absoluteNormals3[0],
                                                        absoluteNormals3[1],
                                                        absoluteNormals3[2] ]])

                            normVertices[0] = blenderToBullet3( normVertices[0] )
                            normVertices[1] = blenderToBullet3( normVertices[1] )
                            normVertices[2] = blenderToBullet3( normVertices[2] )


                            fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f],\n" % (
                                                                            cordVertices[0][0],
                                                                            cordVertices[0][1],
                                                                            cordVertices[0][2],
                                                                            normVertices[0][0],
                                                                            normVertices[0][1],
                                                                            normVertices[0][2],
                                                                            colorList[poly.material_index][0],
                                                                            colorList[poly.material_index][1],
                                                                            colorList[poly.material_index][2]))

                            fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f],\n" % (
                                                                            cordVertices[1][0],
                                                                            cordVertices[1][1],
                                                                            cordVertices[1][2],
                                                                            normVertices[1][0],
                                                                            normVertices[1][1],
                                                                            normVertices[1][2],
                                                                            colorList[poly.material_index][0],
                                                                            colorList[poly.material_index][1],
                                                                            colorList[poly.material_index][2]))

                            fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f]" % (
                                                                            cordVertices[2][0],
                                                                            cordVertices[2][1],
                                                                            cordVertices[2][2],
                                                                            normVertices[2][0],
                                                                            normVertices[2][1],
                                                                            normVertices[2][2],
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
                    fo.write("\t\t\"xpos\":%f,\n" % obj.location[0])
                    fo.write("\t\t\"ypos\":%f,\n" % obj.location[2])
                    fo.write("\t\t\"zpos\":%f,\n" % (-1.0*obj.location[1]))
                    fo.write("\t\t\"xrot\":%f,\n" % obj.rotation_euler[0])
                    fo.write("\t\t\"yrot\":%f,\n" % obj.rotation_euler[2])
                    fo.write("\t\t\"zrot\":%f,\n" % obj.rotation_euler[1])
                    fo.write("\t\t\"wqat\":%f,\n" % obj.rotation_quaternion[0])
                    fo.write("\t\t\"xqat\":%f,\n" % obj.rotation_quaternion[1])
                    fo.write("\t\t\"yqat\":%f,\n" % obj.rotation_quaternion[3])
                    fo.write("\t\t\"zqat\":%f,\n" % (-1.0*obj.rotation_quaternion[2]))
                    fo.write("\t\t\"xscl\":%f,\n" % obj.scale[0])
                    fo.write("\t\t\"yscl\":%f,\n" % obj.scale[2])
                    fo.write("\t\t\"zscl\":%f,\n" % obj.scale[1])


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
                                                                        cordList[poly.vertices[0]][0],
                                                                        cordList[poly.vertices[0]][2],
                                                                        (-1.0*cordList[poly.vertices[0]][1]),
                                                                        normList[poly.vertices[0]][0],
                                                                        normList[poly.vertices[0]][2],
                                                                        (-1.0*normList[poly.vertices[0]][1]),
                                                                        colorList[poly.material_index][0],
                                                                        colorList[poly.material_index][1],
                                                                        colorList[poly.material_index][2]))

                        fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f],\n" % (
                                                                        cordList[poly.vertices[1]][0],
                                                                        cordList[poly.vertices[1]][2],
                                                                        (-1.0*cordList[poly.vertices[1]][1]),
                                                                        normList[poly.vertices[1]][0],
                                                                        normList[poly.vertices[1]][2],
                                                                        (-1.0*normList[poly.vertices[1]][1]),
                                                                        colorList[poly.material_index][0],
                                                                        colorList[poly.material_index][1],
                                                                        colorList[poly.material_index][2]))

                        fo.write("\t\t\t[%f, %f, %f, %f, %f, %f, %f, %f, %f]" % (
                                                                        cordList[poly.vertices[2]][0],
                                                                        cordList[poly.vertices[2]][2],
                                                                        (-1.0*cordList[poly.vertices[2]][1]),
                                                                        normList[poly.vertices[2]][0],
                                                                        normList[poly.vertices[2]][2],
                                                                        (-1.0*normList[poly.vertices[2]][1]),
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
