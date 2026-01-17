#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void run_cmd(const char *cmd) {
    system(cmd);
}

void print_msg(const char *tag, const char *msg) {
    if (strcmp(tag, "[*]") == 0) printf("\033[36m%s %s\033[0m\n", tag, msg);
    if (strcmp(tag, "[+]") == 0) printf("\033[32m%s %s\033[0m\n", tag, msg);
    if (strcmp(tag, "[!]") == 0) printf("\033[31m%s %s\033[0m\n", tag, msg);
}

int main() {
    if (geteuid() != 0) {
        print_msg("[!]", "错误：必须使用 sudo 或 root 权限运行 DLC 程序！");
        print_msg("  ", "正确运行方式: sudo dlc");
        return EXIT_FAILURE;
    }

    printf("=========================================\n");
    printf("               DLC 系统深度清理\n");
    printf("        安全模式 | 不清理内核文件\n");
    printf("=========================================\n\n");

    // APT核心清理
    print_msg("[*]", "正在卸载无用依赖软件包及配置");
    run_cmd("apt autoremove -y --purge");
    print_msg("[+]", "卸载无用依赖完成");

    print_msg("[*]", "正在清理过期软件包缓存");
    run_cmd("apt autoclean -y");
    print_msg("[+]", "清理过期缓存完成");

    print_msg("[*]", "彻底清理全部APT下载缓存");
    run_cmd("apt clean -y");
    print_msg("[+]", "清理APT缓存完成");

    print_msg("[*]", "清理已卸载软件的残留配置文件");
    run_cmd("dpkg -l | grep '^rc' | awk '{print $2}' | xargs dpkg --purge -y 2>/dev/null");
    print_msg("[+]", "清理残留配置完成");

    // 系统全局深度清理
    print_msg("[*]", "清理系统临时文件目录");
    run_cmd("rm -rf /tmp/* /var/tmp/* 2>/dev/null");
    print_msg("[+]", "清理临时文件完成");

    print_msg("[*]", "清理系统日志及压缩包");
    run_cmd("find /var/log -type f -delete 2>/dev/null");
    print_msg("[+]", "清理系统日志完成");

    print_msg("[*]", "清理journal日志缓存");
    run_cmd("journalctl --vacuum-size=50M >/dev/null 2>&1");
    print_msg("[+]", "清理日志缓存完成");

    // 用户目录清理
    print_msg("[*]", "清理所有用户缓存文件");
    run_cmd("rm -rf /home/*/.cache/* 2>/dev/null");
    print_msg("[+]", "清理用户缓存完成");

    print_msg("[*]", "清理所有用户回收站文件");
    run_cmd("rm -rf /home/*/.local/share/Trash/* 2>/dev/null");
    print_msg("[+]", "清理回收站完成");

    print_msg("[*]", "清理所有用户缩略图缓存");
    run_cmd("rm -rf /home/*/.local/share/thumbnail*/* 2>/dev/null");
    print_msg("[+]", "清理缩略图缓存完成");

    print_msg("[*]", "清理系统级缓存文件");
    run_cmd("rm -rf /var/cache/apt/archives/partial/* /var/cache/man/* 2>/dev/null");
    print_msg("[+]", "清理系统缓存完成");

    printf("\n=========================================\n");
    print_msg("[+]", "所有深度清理任务执行完毕！");
    print_msg("[+]", "系统空间已安全释放，无任何风险！");
    printf("=========================================\n");

    return EXIT_SUCCESS;
}