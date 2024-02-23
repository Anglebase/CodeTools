#include "../logger.hxx"

// 基本使用
// Basic usage methods
int main()
{
    // 基本输出(默认输出为控制台)
    // 它们不会导致IO阻塞
    // Basic output (default output is console)
    // They will not cause IO blocking
    logger.debug("debug message");
    logger.info("info message");
    logger.warning("warning message");
    logger.error("error message");

    // 输出重定向(该函数会引发阻塞)
    // Output redirection(This function will cause blocking)
    logger.redirect(DEBUG_level, NEW_OUTPUT("./debug_output_test.txt"));
    logger.debug("after redirect output1");
    logger.debug("after redirect output2");
    logger.debug("after redirect output3");
    logger.debug("after redirect output4");

    // 输出重定向为默认
    // Output redirection to default
    logger.redirect(DEBUG_level, DEFAULT_OUTPUT);
    logger.debug("after redirect to default.");

    // 输出等级修改(该函数会引发阻塞)
    // Output level modification(This function will cause blocking)
    logger.setlesslevel(WARNING_level);
    logger.debug("after Output level modification: debug message");
    logger.info("after Output level modification: info message");
    logger.warning("after Output level modification: warning message");
    logger.error("after Output level modification: error message");
    // level: error > warning > info > debug
    logger.setlesslevel(DEBUG_level);

    // 阻塞主线程等待异步IO输出执行完成
    // Blocking the main thread waiting for asynchronous IO output execution to complete
    using namespace std::chrono_literals;
    while (!logger.finish())
        std::this_thread::sleep_for(20ms);

    return 0;
}