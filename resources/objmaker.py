

def main():
    size = 10
    scale = 0.1
    extended_points = []
    for i in range(-size//2,size//2+1):
        extended_points.append((round(i,2),0,size//2))
        extended_points.append((round(i,2),0,-size//2))
        
    for i in range(-size//2,size//2+1):
        extended_points.append((size//2,0,round(i,2)))
        extended_points.append((-size//2,0,round(i,2)))
    
    extended_points = [(round(scale*a,4), round(scale*b,4), round(scale*c,4)) for a,b,c in extended_points]
    normal = (0,1,0)

    print("GLfloat vertices[] = {")
    newline = 0
    for p in extended_points:
        for val in [x for x in p] + [0.0, 1.0, 0.0] + [round(p[0]/scale/size+0.5,4), round(p[1]/scale/size+0.5,4)]:
            print(f"{val}f,",end=" ")
        newline += 1
        if newline % 2 == 0:
            print("")
        else:
            print("\t\t",end="")
    
    print("};")

    # slices
    print("GLuint indices[] = {")
    newline = 0
    for i in range(0,2*(len(extended_points)//2),2):
        if newline % 2 < 1:
            print(f"{i}, {i+1}",end=", ")
        else:
            print(f"{i+1}, {i}",end=", ")

        newline += 1
        if newline % 2 == 0:
            print("")
    print("}")
    




if __name__ == "__main__":
    main()