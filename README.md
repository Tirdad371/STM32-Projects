# STM32 Projects

Example projects for the STM32F407VG Discovery board, built with STM32CubeIDE.
Some projects also include a Keil uVision 5 project.

## Projects

| Project | Description |
|---|---|
| [1-ALCD](1-ALCD) | 16x2 character LCD driver (4-bit mode, HD44780) |
| [2-UART](2-UART) | UART communication examples (see below) |

### 2-UART

| Project | Description |
|---|---|
| [1-UARTEcho](2-UART/1-UARTEcho) | Echo received bytes back, polling mode |
| [2-UARTEchoInterrupt](2-UART/2-UARTEchoInterrupt) | Echo using receive interrupts |
| [3-UARTEchoDMA](2-UART/3-UARTEchoDMA) | Echo using DMA transfers |
| [4-UART-ReadLine-IT-SingleBuff](2-UART/4-UART-ReadLine-IT-SingleBuff) | Read a line of text with interrupts and one buffer |
| [5-UART-ReadLine-IT-SingleBuff-Advanced](2-UART/5-UART-ReadLine-IT-SingleBuff-Advanced) | Same as 4, with an extended implementation |
| [6-UART-ReadLine-IT-MultiBuff](2-UART/6-UART-ReadLine-IT-MultiBuff) | Read lines with interrupts and multiple buffers |
| [7-UART-ReadLine-MultiBuff-Advanced](2-UART/7-UART-ReadLine-MultiBuff-Advanced) | Same as 6, with an extended implementation |

## Hardware

- STM32F407G-DISC1 board, programmed over the on-board ST-Link
- 16x2 HD44780-compatible LCD (for 1-ALCD)
- USB-to-serial adapter or terminal program (for the UART projects)

## How to open a project

1. Clone the repo: `git clone https://github.com/Tirdad371/STM32-Projects.git`
2. In STM32CubeIDE: File -> Import -> Existing Projects into Workspace
3. Select the project folder and build
