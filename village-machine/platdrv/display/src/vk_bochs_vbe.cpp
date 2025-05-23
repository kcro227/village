//###########################################################################
// vk_bochs_vbe.cpp
// Definitions of the functions that manage Bochs VBE
//
// $Copyright: Copyright (C) village
//###########################################################################
#include "vk_bochs_vbe.h"
#include "vk_kernel.h"


#define VBE_DISPI_IOPORT_INDEX       (0x01CE)
#define VBE_DISPI_IOPORT_DATA        (0x01CF)

#define VBE_DISPI_INDEX_ID           (0)
#define VBE_DISPI_INDEX_XRES         (1)
#define VBE_DISPI_INDEX_YRES         (2)
#define VBE_DISPI_INDEX_BPP          (3)
#define VBE_DISPI_INDEX_ENABLE       (4)
#define VBE_DISPI_INDEX_BANK         (5)
#define VBE_DISPI_INDEX_VIRT_WIDTH   (6)
#define VBE_DISPI_INDEX_VIRT_HEIGHT  (7)
#define VBE_DISPI_INDEX_X_OFFSET     (8)
#define VBE_DISPI_INDEX_Y_OFFSET     (9)

#define VBE_DISPI_DISABLED           (0x00)
#define VBE_DISPI_ENABLED            (0x01)
#define VBE_DISPI_LFB_ENABLED        (0x40)
#define VBE_DISPI_NOCLEARMEM         (0x80)

#define VBE_DISPI_BPP_4              (0x04)
#define VBE_DISPI_BPP_8              (0x08)
#define VBE_DISPI_BPP_15             (0x0F)
#define VBE_DISPI_BPP_16             (0x10)
#define VBE_DISPI_BPP_24             (0x18)
#define VBE_DISPI_BPP_32             (0x20)

#define VBE_DISPI_ID0                (0xB0C0)
#define VBE_DISPI_ID1                (0xB0C1)
#define VBE_DISPI_ID2                (0xB0C2)
#define VBE_DISPI_ID3                (0xB0C3)
#define VBE_DISPI_ID4                (0xB0C4)
#define VBE_DISPI_ID5                (0xB0C5)


/// @brief Constructor
BochsVBE::BochsVBE()
{
}


/// @brief Destructor
BochsVBE::~BochsVBE()
{
}


/// @brief Set data
/// @param data 
void BochsVBE::SetData(void* data)
{
    config = *((Config*)data);
}


/// @brief BochsVBE write data
/// @param reg 
/// @param value 
inline void BochsVBE::WriteData(uint32_t reg, uint16_t value)
{
    config.vmap[reg] = value;
}


/// @brief BochsVBE read data
/// @param reg 
/// @return 
inline uint16_t BochsVBE::ReadData(uint32_t reg)
{
    return config.vmap[reg];
}


/// @brief BochsVBE write reg
/// @param reg 
/// @param value 
inline void BochsVBE::WriteReg(uint16_t reg, uint16_t value)
{
    PortWordOut(VBE_DISPI_IOPORT_INDEX, reg);
    PortWordOut(VBE_DISPI_IOPORT_DATA, value);
}


/// @brief BochsVBE read reg
/// @param reg 
/// @return 
inline uint16_t BochsVBE::ReadReg(uint16_t reg)
{
    PortWordOut(VBE_DISPI_IOPORT_INDEX, reg);
    return PortWordIn(VBE_DISPI_IOPORT_DATA);
}


/// @brief Check is available
/// @return 
bool BochsVBE::IsBochsVBEAvailable()
{
    return (ReadReg(VBE_DISPI_INDEX_ID) >= VBE_DISPI_ID4);
}


/// @brief Set video mode
/// @param width 
/// @param height 
/// @param bitdepth 
/// @param isUseLinearFrameBuffer 
/// @param isClearVideoMemory 
void BochsVBE::SetVideoMode(uint16_t width, uint16_t height, uint16_t bitdepth, bool isUseLinearFrameBuffer, bool isClearVideoMemory)
{
    WriteReg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    WriteReg(VBE_DISPI_INDEX_XRES, width);
    WriteReg(VBE_DISPI_INDEX_YRES, height);
    WriteReg(VBE_DISPI_INDEX_BPP,  bitdepth);
    WriteReg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED |
            (isUseLinearFrameBuffer ? VBE_DISPI_LFB_ENABLED : 0) |
            (isClearVideoMemory ? 0 : VBE_DISPI_NOCLEARMEM));
}


/// @brief Set bank
/// @param bankNumber 
void BochsVBE::SetBank(uint16_t bankNumber)
{
    WriteReg(VBE_DISPI_INDEX_BANK, bankNumber);
}


/// @brief BochsVBE setup
bool BochsVBE::Setup()
{
    info.width    = 1024;
    info.height   = 768;
    info.bitdepth = 0x10; // 16 bpp

    if (IsBochsVBEAvailable())
    {
        //Set video mode
        SetVideoMode(info.width, info.height, info.bitdepth, true, true);
        
        //Get PCI device 0x1234:0x1111 BAR 0
        config.vmap = (uint16_t*)pci.ReadBAR(0x1234, 0x1111, 0);

        return true;
    }

    return false;
}


/// @brief BochsVBE draw point
/// @param x 
/// @param y 
/// @param color 
void BochsVBE::DrawPoint(uint32_t x, uint32_t y, uint32_t color)
{
    uint32_t reg = x + y * info.width;
    WriteData(reg, color);
}


/// @brief BochsVBE read point
/// @param x 
/// @param y 
/// @return 
uint32_t BochsVBE::ReadPoint(uint32_t x, uint32_t y)
{
    uint32_t reg = x + y * info.width;
    return ReadData(reg);
}


/// @brief BochsVBE fill area
/// @param sx 
/// @param sy 
/// @param ex 
/// @param ey 
/// @param color 
void BochsVBE::Fill(uint32_t sx, uint32_t sy, uint32_t ex, uint32_t ey, uint32_t color)
{
    for (uint32_t y = sy; y <= ey; y++)
    {
        for (uint32_t x = sx; x <= ex; x++)
        {
            DrawPoint(x, y, color);
        }
    }
}


/// @brief BochsVBE fill area
/// @param sx 
/// @param sy 
/// @param ex 
/// @param ey 
/// @param pxmap 
void BochsVBE::Fill(uint32_t sx, uint32_t sy, uint32_t ex, uint32_t ey, uint8_t* pxmap)
{
    uint16_t* buf16 = (uint16_t*)pxmap;

    for (uint32_t y = sy; y <= ey; y++)
    {
        for (uint32_t x = sx; x <= ex; x++)
        {
            DrawPoint(x, y, *buf16);
            buf16++;
        }
    }
}


/// @brief BochsVBE clear
/// @param color 
void BochsVBE::Clear(uint32_t color)
{
    Fill(0, 0, info.width, info.height, color);
}


/// @brief BochsVBE exit
void BochsVBE::Exit()
{

}


/// @brief Open
bool BochsVBE::Open()
{
    return Setup();
}


/// @brief IOCtrl
/// @param cmd 
/// @param data 
/// @return 
int BochsVBE::IOCtrl(uint8_t cmd, void* data)
{
    FBDriver** fbdev = (FBDriver**)data;

    *fbdev = this;

    return 0;
}


/// @brief Close
void BochsVBE::Close()
{
    Exit();
}


/// @brief Probe
/// @param device 
/// @return 
bool BochsVBEDrv::Probe(PlatDevice* device)
{
    device->Attach(new BochsVBE());
    kernel->device.RegisterFBDevice((FBDriver*)device->GetDriver());
    return true;
}


/// @brief Remove
/// @param device 
/// @return 
bool BochsVBEDrv::Remove(PlatDevice* device)
{
    kernel->device.UnregisterFBDevice((FBDriver*)device->GetDriver());
    delete (BochsVBE*)device->GetDriver();
    device->Detach();
    return true;
}


///Register driver
REGISTER_PLAT_DRIVER(new BochsVBEDrv(), bochsVBE, bochsVBEDrv);
