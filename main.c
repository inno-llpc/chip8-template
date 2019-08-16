#include <stdio.h>
#include <time.h>

#include "helpers.h"

#define SCALE_FACTOR 20


struct cpu {
    uint8_t v[0x10];
    uint16_t i;

    uint16_t pc;
    uint16_t sp;

    uint16_t dt;
    uint16_t st;
};

struct cpu cpu;
uint8_t mem[0x1000];
uint16_t stack[0x30];
uint8_t disp[64][32];


void
load_file(char *nm)
{
    FILE *fp =  fopen(nm, "rb");

    if (!fp) {
        printf("Can't open file '%s'\n", nm);
        exit(1);
    }

    for (int i = 0x200; i < 0x1000; i++) {
        int b = fgetc(fp);

        if (b == EOF)
            break;
        mem[i] = b;
    }
    fclose(fp);
}

void
load_fonts()
{
    const uint8_t fonts[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    for (int i = 0; i < (int)sizeof(fonts); i++)
        mem[i] = fonts[i];
}

void
set_pixel(int x, int y)
{
    if (x >= 64 || y >= 32)
        return;
    if (disp[x][y]) {
        disp[x][y] = 0;
        cpu.v[0xF] = 1;
    } else {
        disp[x][y] = 1;
    }
}

void
draw_pixel(int x, int y)
{
    for (int i = 0; i < SCALE_FACTOR; i++) {
        for (int j = 0; j < SCALE_FACTOR; j++)
            helper_draw_point(x*SCALE_FACTOR+i,
                              y*SCALE_FACTOR+j);
    }
}

void
update_screen()
{
    helper_clear_screen(0, 0, 0, 0);
    helper_set_draw_color(255, 255, 255, 0);

    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 32; j++) {
            if (disp[i][j])
                draw_pixel(i, j);
        }
    }
    helper_update_screen();
}


void
draw_sprite(uint16_t addr, int x, int y, int n)
{
    cpu.v[0xF] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 8; j++) {
            if ((mem[addr+i] >> j) & 1)
                set_pixel(x + (7-j), y + i);
        }
    }
    update_screen();
}

void
cpu_iter()
{
    uint16_t inst = (mem[cpu.pc] << 8) + mem[cpu.pc+1];

    printf("%03x:\t%04x\n", cpu.pc, inst);
    cpu.pc += 2;

    switch (inst >> 12) {
    case 0:
        switch (inst) {
        case 0x00EE:
            cpu.sp--;
            cpu.pc = stack[cpu.sp];
            break;
        default:
            printf("Unknown instruction!\n");
            exit(1);
        }
        break;
    case 1:
        cpu.pc = inst & 0xFFF;
        break;
    case 2:
        stack[cpu.sp] = cpu.pc;
        cpu.sp++;
        cpu.pc = inst & 0xFFF;
        break;
    case 3: {
        int x = (inst >> 8) & 0xF;
        int kk = inst & 0xFF;

        if (cpu.v[x] == kk)
            cpu.pc += 2;
        break;
    }
    case 4: {
        int x = (inst >> 8) & 0xF;
        int kk = inst & 0xFF;

        if (cpu.v[x] != kk)
            cpu.pc += 2;
        break;
    }
    case 6: {
        int x = (inst >> 8) & 0xF;
        int kk = inst & 0xFF;

        cpu.v[x] = kk;
        break;
    }
    case 7: {
        int x = (inst >> 8) & 0xF;
        int kk = inst & 0xFF;

        cpu.v[x] = cpu.v[x] + kk;
        break;
    }
    case 8: {
        int x = (inst >> 8) & 0xF;
        int y = (inst >> 4) & 0xF;
        int type = inst & 0xF;

        switch (type){
        case 0:
            cpu.v[x] = cpu.v[y];
            break;
        case 1:
            cpu.v[x] = cpu.v[x] | cpu.v[y];
            break;
        case 2:
            cpu.v[x] = cpu.v[x] & cpu.v[y];
            break;
        case 3:
            cpu.v[x] = cpu.v[x] ^ cpu.v[y];
            break;
        case 4:
            if ((int)cpu.v[x] + (int)cpu.v[y] > 255)
                cpu.v[0xF] = 1;
            else
                cpu.v[0xF] = 0;
            cpu.v[x] = cpu.v[x] + cpu.v[y];
            break;
        case 5:
            if ((int)cpu.v[x] > (int)cpu.v[y])
                cpu.v[0xF] = 1;
            else
                cpu.v[0xF] = 0;
            cpu.v[x] = cpu.v[x] - cpu.v[y];
            break;
        default:
            printf("Unknown instruction!\n");
            exit(1);
        }
        break;
    }
    case 0xA: {
        cpu.i = inst & 0xFFF;
        break;
    }
    case 0xD: {
        int x = (inst >> 8) & 0xF;
        int y = (inst >> 4) & 0xF;
        int n = inst & 0xF;

        draw_sprite(cpu.i, cpu.v[x], cpu.v[y], n);
        break;
    }
    case 0xC: {
        int x = (inst >> 8) & 0xF;
        int kk = inst & 0xFF;

        cpu.v[x] = random() & kk;
        break;
    }
    case 0xE: {
        int x = (inst >> 8) & 0xF;
        int type = inst & 0xFF;

        switch (type) {
        case 0xA1:
            if (!helper_is_key_pressed(cpu.v[x]))
                cpu.pc += 2;
            break;
        default:
            printf("Unknown instruction!\n");
            exit(1);
        }
        break;
    }
    case 0xF: {
        int x = (inst >> 8) & 0xF;
        int type = inst & 0xFF;

        switch (type) {
        case 0x07:
            cpu.v[x] = cpu.dt;
            break;
        case 0x15:
            cpu.dt = cpu.v[x];
            break;
        case 0x18:
            cpu.st = cpu.v[x];
            break;
        case 0x29:
            cpu.i = 5 * cpu.v[x];
            break;
        case 0x33:
            mem[cpu.i] = cpu.v[x]/100;
            mem[cpu.i+1] = cpu.v[x] / 10 % 10;
            mem[cpu.i+2] = cpu.v[x] % 10;
            break;
        case 0x65:
            for (int i = 0; i <= x; i++)
                cpu.v[x] = mem[cpu.i + i];
            break;
        default:
            printf("Unknown instruction!\n");
            exit(1);
        }
        break;
    }
    default:
        printf("Unknown instruction!\n");
        exit(1);
    }
}

void
main_loop()
{
    uint32_t inst_time = MS_IN_S/400;

    while (!helper_is_user_exit()) {
        uint32_t start, end, diff;

        start = helper_time();
        cpu_iter();
        end = helper_time();

        diff = end - start;

        if (inst_time > diff)
            helper_sleep(inst_time - diff);
    }
}

void
cpu_init()
{
    cpu.pc = 0x200;
    cpu.sp = 0;
}

void
timers()
{
    if (cpu.dt > 0)
        cpu.dt--;
    if (cpu.st > 0)
        cpu.st--;
    helper_beep(cpu.st);
}

int
main(int argc, char **argv)
{
    if (argc < 2)
        return 1;

    load_file(argv[1]);
    load_fonts();

    helper_initgfx(64*SCALE_FACTOR, 32*SCALE_FACTOR);
    helper_add_timer(MS_IN_S/60, timers);
    srand(time(NULL));
    cpu_init();
    main_loop();

    return 0;
}

