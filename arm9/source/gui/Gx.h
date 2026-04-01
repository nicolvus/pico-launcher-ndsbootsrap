#pragma once
#include <libtwl/gfx/gfx3dCmd.h>
#include "core/math/ColorConverter.h"
#include "core/math/fixed.h"

namespace Gx
{
    static inline void MtxMode(GxMtxMode mode)
    {
        gx_mtxMode(mode);
    }

    static inline void MtxIdentity()
    {
        gx_mtxIdentity();
    }

    static inline void MtxLoad44(const mtx44_t* matrix)
    {
        gx_mtxLoad44(matrix);
    }

    static inline void MtxLoad43(const mtx43_t* matrix)
    {
        gx_mtxLoad43(matrix);
    }

    static inline void MtxMult44(const mtx44_t* matrix)
    {
        gx_mtxMult44(matrix);
    }

    static inline void MtxMult43(const mtx43_t* matrix)
    {
        gx_mtxMult43(matrix);
    }

    static inline void MtxMult33(const mtx33_t* matrix)
    {
        gx_mtxMult33(matrix);
    }

    static inline void MtxTranslate(fix32<12> x, fix32<12> y, fix32<12> z)
    {
        gx_mtxTranslate(x.GetRawValue(), y.GetRawValue(), z.GetRawValue());
    }

    static inline void MtxScale(fix32<12> x, fix32<12> y, fix32<12> z)
    {
        gx_mtxScale(x.GetRawValue(), y.GetRawValue(), z.GetRawValue());
    }

    static inline void MtxPush()
    {
        gx_mtxPush();
    }

    static inline void MtxPop(int count)
    {
        gx_mtxPop(count);
    }

    static inline void PolygonAttr(GxLightMask lightMask, GxPolygonMode polygonMode,
        GxDisplayMode displayMode, bool xluDepthUpdate, bool clipFarPlane, bool render1Dot,
        GxDepthFunc depthFunc, bool fogEnable, u32 alpha, u32 polygonId)
    {
        gx_polygonAttr(lightMask, polygonMode, displayMode, xluDepthUpdate, clipFarPlane,
            render1Dot, depthFunc, fogEnable, alpha, polygonId);
    }

    static inline void PolygonAttr(u32 polygonAttr)
    {
        REG_GX_POLYGON_ATTR = polygonAttr;
    }

    static inline void TexImageParam(u16 address, bool repeatS, bool repeatT,
        bool flipS, bool flipT, GxTexSize sizeS, GxTexSize sizeT, GxTexFormat format,
        bool color0Transparent, GxTexGen texGen)
    {
        gx_texImageParam(address, repeatS, repeatT, flipS, flipT,
            sizeS, sizeT, format, color0Transparent, texGen);
    }

    static inline void TexPlttBase(u32 address)
    {
        gx_texPlttBase(address);
    }

    static inline void TexImageParam(u32 texImageParam)
    {
        REG_GX_TEXIMAGE_PARAM = texImageParam;
    }

    static inline void TexCoord(fix16<4> s, fix16<4> t)
    {
        gx_texCoord(s.GetRawValue(), t.GetRawValue());
    }

    static inline void Normal(int x, int y, int z)
    {
        gx_normal(x, y, z);
    }

    static inline void Color(const Rgb<5, 5, 5>& color)
    {
        gx_color(ColorConverter::ToXBGR555(color));
    }

    static inline void Color(u32 color)
    {
        gx_color(color);
    }

    static inline void Color(u32 r, u32 g, u32 b)
    {
        Color(Rgb<5, 5, 5>(r, g, b));
    }

    static inline void Vtx16(fix16<12> x, fix16<12> y, fix16<12> z)
    {
        gx_vtx16(x.GetRawValue(), y.GetRawValue(), z.GetRawValue());
    }

    static inline void Vtx10(fix16<6> x, fix16<6> y, fix16<6> z)
    {
        gx_vtx10(x.GetRawValue(), y.GetRawValue(), z.GetRawValue());
    }

    static inline void VtxXY(fix16<12> x, fix16<12> y)
    {
        gx_vtxXY(x.GetRawValue(), y.GetRawValue());
    }

    static inline void VtxXZ(fix16<12> x, fix16<12> z)
    {
        gx_vtxXZ(x.GetRawValue(), z.GetRawValue());
    }

    static inline void VtxYZ(fix16<12> y, fix16<12> z)
    {
        gx_vtxYZ(y.GetRawValue(), z.GetRawValue());
    }

    static inline void VtxDiff(fix16<12> x, fix16<12> y, fix16<12> z)
    {
        gx_vtxDiff(x.GetRawValue(), y.GetRawValue(), z.GetRawValue());
    }

    static inline void Begin(GxPrimitiveType type)
    {
        gx_beginVtxs(type);
    }

    static inline void End()
    {
        gx_endVtxs();
    }

    static inline void LightVector(int light, int x, int y, int z)
    {
        gx_lightVector(light, x, y, z);
    }

    static inline void LightColor(int light, u32 color)
    {
        gx_lightColor(light, color);
    }

    static inline void DiffuseAmbient(u32 diffuse, bool setDiffuseVtxColor, u32 ambient)
    {
        gx_diffuseAmbient(diffuse, setDiffuseVtxColor, ambient);
    }

    static inline void DiffuseAmbient(const Rgb<5, 5, 5>& diffuse, bool setDiffuseVtxColor, const Rgb<5, 5, 5>& ambient)
    {
        gx_diffuseAmbient(ColorConverter::ToXBGR555(diffuse), setDiffuseVtxColor, ColorConverter::ToXBGR555(ambient));
    }

    static inline void SpecularEmission(u32 specular, bool useShininessTable, u32 emission)
    {
        gx_specularEmission(specular, useShininessTable, emission);
    }

    static inline void SpecularEmission(const Rgb<5, 5, 5>& specular, bool useShininessTable, const Rgb<5, 5, 5>& emission)
    {
        gx_specularEmission(ColorConverter::ToXBGR555(specular), useShininessTable, ColorConverter::ToXBGR555(emission));
    }

    static inline void SwapBuffers(GxXluSortMode xluSortMode, GxDepthMode depthMode)
    {
        gx_swapBuffers(xluSortMode, depthMode);
    }

    static inline void Viewport(int x1, int y1, int x2, int y2)
    {
        gx_viewport(x1, y1, x2, y2);
    }
};
