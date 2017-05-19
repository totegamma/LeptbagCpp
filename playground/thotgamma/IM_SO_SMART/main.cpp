#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


extern "C" void hoge(){
	printf("ﾋﾟｮﾋﾟｮw\n");
}

int main() {
    printf("+main()\n");

    void *lh = dlopen("libdll.so", RTLD_LAZY);
    if (!lh)
    {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        exit(1);
    }
    printf("libdll.so is loaded\n");

    int (*fn)() = (int (*)())dlsym(lh, "dll");
    char *error = dlerror();
    if (error)
    {
        fprintf(stderr, "dlsym error: %s\n", error);
        exit(1);
    }
    printf("dll() function is found\n");

    (*fn)();

    printf("unloading libdll.so\n");
    dlclose(lh);

    printf("-main()\n");
    return 0;
}
