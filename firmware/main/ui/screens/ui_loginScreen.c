#include "ui_loginScreen.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui_cfg.h"
#include "ui.h"
#include "alarm_mgr.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_FAILS 5
#define LOCK_MS 30000U
static const char s_tecnico[] = {'T',(char)0xC3,(char)0xA9,'c','n','i','c','o',0};

lv_obj_t *ui_loginScreen = NULL;
static const app_user_t *s_target;
static char s_pin[9];
static int s_len, s_fails;
static uint32_t s_lock_until;
static lv_obj_t *s_who, *s_dots, *s_error;
/* Destino tras un login exitoso. NULL = comportamiento por defecto (Configuración
 * -> pantalla General). El llamador lo fija con ui_login_set_destination() DESPUÉS
 * de crear la pantalla (screen_init lo resetea), y se consume en el primer acierto. */
static void (*s_after_login)(void);

void ui_login_set_destination(void (*cb)(void)) { s_after_login = cb; }

static const char *role_name(app_user_role_t r){return r==APP_ROLE_FACTORY?"Fabricante":r==APP_ROLE_ADMIN?"Administrador":s_tecnico;}
static uint32_t role_color(app_user_role_t r){return r==APP_ROLE_FACTORY?0x8b5cf6:r==APP_ROLE_ADMIN?UI_C_BLUE:UI_C_OK;}
static void refresh(void){char b[24]="";for(int i=0;i<s_len&&i<8;i++){b[i*2]='*';if(i<7)b[i*2+1]=' ';}lv_label_set_text(s_dots,b);}
static void select_user(const app_user_t *u){s_target=u;s_len=0;s_pin[0]=0;refresh();lv_label_set_text_fmt(s_who,"%s - %s",u?u->name:"Seleccione usuario",u?role_name(u->role):"");lv_obj_add_flag(s_error,LV_OBJ_FLAG_HIDDEN);}
static void user_cb(lv_event_t *e){select_user(ui_auth_user_at((int)(intptr_t)lv_event_get_user_data(e)));}
static void key_cb(lv_event_t *e){
    char k=(char)(intptr_t)lv_event_get_user_data(e);uint32_t now=lv_tick_get();
    if(s_lock_until&&(int32_t)(now-s_lock_until)<0){lv_label_set_text(s_error,"Acceso bloqueado temporalmente");lv_obj_clear_flag(s_error,LV_OBJ_FLAG_HIDDEN);return;}s_lock_until=0;
    if(k=='<'){if(s_len>0)s_pin[--s_len]=0;refresh();return;}
    if(k=='='){
        if(s_target&&s_len>=4&&ui_auth_login(s_target,s_pin)){s_fails=0;void(*dest)(void)=s_after_login;s_after_login=NULL;if(dest)dest();else ui_open_general_authenticated_cb(NULL);return;}
        s_len=0;s_pin[0]=0;refresh();if(++s_fails>=MAX_FAILS){s_fails=0;s_lock_until=now+LOCK_MS;lv_label_set_text(s_error,"5 intentos - bloqueado 30 segundos");}else lv_label_set_text_fmt(s_error,"PIN incorrecto - intento %d/%d",s_fails,MAX_FAILS);lv_obj_clear_flag(s_error,LV_OBJ_FLAG_HIDDEN);return;
    }
    if(k>='0'&&k<='9'&&s_len<8){s_pin[s_len++]=k;s_pin[s_len]=0;refresh();}
}
static lv_obj_t *key(lv_obj_t *g,const char *txt,char code,int col,int row){lv_obj_t *o=ui_card(g);lv_obj_set_grid_cell(o,LV_GRID_ALIGN_STRETCH,col,1,LV_GRID_ALIGN_STRETCH,row,1);lv_obj_set_style_pad_all(o,0,0);lv_obj_add_flag(o,LV_OBJ_FLAG_CLICKABLE);lv_obj_add_event_cb(o,key_cb,LV_EVENT_CLICKED,(void*)(intptr_t)code);lv_obj_t *kl = (code=='<' || code=='=') ? ui_icon(o,code=='<'?UI_SYM_BACKSPACE:UI_SYM_CHECK,UI_ICON_MD,code=='='?UI_C_OK:UI_C_TEXT) : ui_label(o,txt,UI_FONT_LG,UI_C_TEXT);lv_obj_center(kl);return o;}
static void add_user(lv_obj_t *p,int i){const app_user_t *u=ui_auth_user_at(i);if(!u)return;lv_obj_t *r=ui_card(p);lv_obj_set_size(r,LV_PCT(100),44);lv_obj_set_flex_flow(r,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(r,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_set_style_pad_all(r,7,0);lv_obj_set_style_pad_column(r,8,0);lv_obj_add_flag(r,LV_OBJ_FLAG_CLICKABLE);lv_obj_add_event_cb(r,user_cb,LV_EVENT_CLICKED,(void*)(intptr_t)i);ui_icon_badge(r,u->role==APP_ROLE_ADMIN?UI_SYM_SHIELD_CHECK:UI_SYM_SETTINGS,UI_ICON_SM,role_color(u->role),UI_C_CARD_BG,28);lv_obj_t *b=ui_box(r);lv_obj_set_flex_flow(b,LV_FLEX_FLOW_COLUMN);lv_obj_set_height(b,32);ui_label(b,u->name,UI_FONT_SM,UI_C_TEXT);ui_label(b,role_name(u->role),UI_FONT_XS,UI_C_TEXT_MUTED);}
/* 1.5.33: acceso del FABRICANTE (cuenta unica y OCULTA: no se lista). Igual que
 * MedGuard (ui_login.c): candado arriba a la derecha -> hoja con la passphrase
 * ALFANUMERICA (teclado QWERTY, >= 8) validada contra general.factory.pin
 * (ui_auth_login -> ui_pin_verify), con el mismo bloqueo por intentos que el pad.
 * La hoja es hija FLOTANTE de la pantalla de login (se borra con ella; las
 * pantallas quedan en la pila de ui_nav, por eso al salir solo se oculta). */
static lv_obj_t *s_fac_sheet, *s_fac_ta, *s_fac_err;
static void fac_show_err(const char *t){lv_label_set_text(s_fac_err,t);lv_obj_clear_flag(s_fac_err,LV_OBJ_FLAG_HIDDEN);}
static void fac_hide(void){if(!s_fac_sheet)return;lv_textarea_set_text(s_fac_ta,"");lv_obj_add_flag(s_fac_err,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(s_fac_sheet,LV_OBJ_FLAG_HIDDEN);}
static void fac_try(void){
    uint32_t now=lv_tick_get();
    if(s_lock_until&&(int32_t)(now-s_lock_until)<0){fac_show_err("Acceso bloqueado temporalmente");return;}s_lock_until=0;
    const char *pass=lv_textarea_get_text(s_fac_ta);
    if(strlen(pass)>=8&&ui_auth_login(ui_auth_factory(),pass)){s_fails=0;fac_hide();void(*dest)(void)=s_after_login;s_after_login=NULL;if(dest)dest();else ui_open_general_authenticated_cb(NULL);return;}
    lv_textarea_set_text(s_fac_ta,"");
    if(++s_fails>=MAX_FAILS){s_fails=0;s_lock_until=now+LOCK_MS;fac_show_err("5 intentos - bloqueado 30 segundos");}
    else{char b[48];snprintf(b,sizeof b,"Clave incorrecta - intento %d/%d",s_fails,MAX_FAILS);fac_show_err(b);}
}
static void fac_kb_cb(lv_event_t *e){lv_event_code_t c=lv_event_get_code(e);if(c==LV_EVENT_READY)fac_try();else if(c==LV_EVENT_CANCEL)fac_hide();}
static void fac_close_cb(lv_event_t *e){(void)e;fac_hide();}
static void fac_open_cb(lv_event_t *e){
    (void)e;
    if(!s_fac_sheet){
        s_fac_sheet=ui_box(ui_loginScreen);lv_obj_add_flag(s_fac_sheet,LV_OBJ_FLAG_FLOATING);lv_obj_set_size(s_fac_sheet,LV_PCT(100),LV_PCT(100));lv_obj_align(s_fac_sheet,LV_ALIGN_CENTER,0,0);
        lv_obj_set_style_bg_color(s_fac_sheet,ui_col(UI_C_SCREEN_BG),0);lv_obj_set_style_bg_opa(s_fac_sheet,LV_OPA_COVER,0);lv_obj_add_flag(s_fac_sheet,LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_flex_flow(s_fac_sheet,LV_FLEX_FLOW_COLUMN);lv_obj_set_style_pad_row(s_fac_sheet,6,0);
        lv_obj_t *h=ui_box(s_fac_sheet);lv_obj_set_size(h,LV_PCT(100),LV_SIZE_CONTENT);lv_obj_set_flex_flow(h,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(h,LV_FLEX_ALIGN_SPACE_BETWEEN,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
        ui_label(h,"Acceso de fabricante",UI_FONT_TITLE,UI_C_TEXT);
        lv_obj_t *x=ui_card(h);lv_obj_set_size(x,32,32);lv_obj_set_style_pad_all(x,0,0);lv_obj_add_flag(x,LV_OBJ_FLAG_CLICKABLE);lv_obj_add_event_cb(x,fac_close_cb,LV_EVENT_CLICKED,NULL);lv_obj_center(ui_icon(x,UI_SYM_ARROW_LEFT,UI_ICON_SM,UI_C_TEXT));
        s_fac_ta=lv_textarea_create(s_fac_sheet);lv_obj_set_width(s_fac_ta,LV_PCT(100));lv_textarea_set_one_line(s_fac_ta,true);lv_textarea_set_password_mode(s_fac_ta,true);lv_textarea_set_max_length(s_fac_ta,23);lv_textarea_set_placeholder_text(s_fac_ta,"Passphrase (minimo 8)");
        lv_obj_set_style_bg_color(s_fac_ta,ui_col(UI_C_CARD_BG2),0);lv_obj_set_style_bg_opa(s_fac_ta,LV_OPA_COVER,0);lv_obj_set_style_border_width(s_fac_ta,1,0);lv_obj_set_style_border_color(s_fac_ta,ui_col(UI_C_BORDER),0);lv_obj_set_style_text_color(s_fac_ta,ui_col(UI_C_TEXT),0);lv_obj_set_style_text_font(s_fac_ta,UI_FONT_MD,0);
        s_fac_err=ui_label(s_fac_sheet,"",UI_FONT_XS,UI_C_ALARM_SOFT);lv_obj_add_flag(s_fac_err,LV_OBJ_FLAG_HIDDEN);
        lv_obj_t *kb=lv_keyboard_create(s_fac_sheet);lv_obj_set_width(kb,LV_PCT(100));lv_obj_set_flex_grow(kb,1);lv_keyboard_set_textarea(kb,s_fac_ta);lv_obj_add_event_cb(kb,fac_kb_cb,LV_EVENT_ALL,NULL);
    }
    lv_obj_clear_flag(s_fac_sheet,LV_OBJ_FLAG_HIDDEN);lv_obj_move_foreground(s_fac_sheet);
}
void ui_loginScreen_screen_init(void){
    s_target=NULL;s_len=0;s_pin[0]=0;s_after_login=NULL;s_fac_sheet=s_fac_ta=s_fac_err=NULL;
    /* #4: al entrar a config (login) no debe pitar el buzzer por WARNING/fallo
     * tecnico; un ALERT critico de presion si sigue sonando. Se limpia al volver
     * al dashboard (main.c). */
    alarm_mgr_set_audio_inhibit_noncritical(true);
    ui_loginScreen=ui_screen_base();lv_obj_set_flex_flow(ui_loginScreen,LV_FLEX_FLOW_COLUMN);lv_obj_set_style_pad_all(ui_loginScreen,8,0);lv_obj_set_style_pad_row(ui_loginScreen,6,0);
    lv_obj_t *hdr=ui_nav_header(ui_loginScreen,"Acceso a configuracion");
    /* Candado discreto (sin texto) = acceso oculto del Fabricante. */
    lv_obj_t *fac=ui_box(hdr);lv_obj_set_size(fac,32,32);lv_obj_add_flag(fac,LV_OBJ_FLAG_CLICKABLE);lv_obj_set_ext_click_area(fac,8);lv_obj_add_event_cb(fac,fac_open_cb,LV_EVENT_CLICKED,NULL);lv_obj_center(ui_icon(fac,UI_SYM_LOCK,UI_ICON_SM,UI_C_WARN_DIM));
    lv_obj_t *body=ui_box(ui_loginScreen);lv_obj_set_width(body,LV_PCT(100));lv_obj_set_flex_grow(body,1);lv_obj_set_flex_flow(body,LV_FLEX_FLOW_ROW);lv_obj_set_style_pad_column(body,10,0);
    lv_obj_t *left=ui_box(body);lv_obj_set_size(left,220,LV_PCT(100));lv_obj_set_flex_flow(left,LV_FLEX_FLOW_COLUMN);lv_obj_set_style_pad_row(left,5,0);ui_label(left,"Seleccione su cuenta",UI_FONT_MD,UI_C_TEXT);for(int i=0;i<ui_auth_user_count();i++)add_user(left,i);
    lv_obj_t *right=ui_box(body);lv_obj_set_flex_grow(right,1);lv_obj_set_height(right,LV_PCT(100));lv_obj_set_flex_flow(right,LV_FLEX_FLOW_COLUMN);lv_obj_set_style_pad_row(right,5,0);s_who=ui_label(right,"Seleccione usuario",UI_FONT_SM,UI_C_TEXT_2);s_dots=ui_label(right,"",UI_FONT_MD,UI_C_OK);s_error=ui_label(right,"",UI_FONT_XS,UI_C_ALARM_SOFT);lv_obj_add_flag(s_error,LV_OBJ_FLAG_HIDDEN);
    static int32_t cols[]={LV_GRID_FR(1),LV_GRID_FR(1),LV_GRID_FR(1),LV_GRID_TEMPLATE_LAST};static int32_t rows[]={LV_GRID_FR(1),LV_GRID_FR(1),LV_GRID_FR(1),LV_GRID_FR(1),LV_GRID_TEMPLATE_LAST};lv_obj_t *g=ui_box(right);lv_obj_set_width(g,LV_PCT(100));lv_obj_set_flex_grow(g,1);lv_obj_set_grid_dsc_array(g,cols,rows);lv_obj_set_layout(g,LV_LAYOUT_GRID);lv_obj_set_style_pad_all(g,0,0);lv_obj_set_style_pad_row(g,4,0);lv_obj_set_style_pad_column(g,4,0);const char *d[] = {"1","2","3","4","5","6","7","8","9"};for(int i=0;i<9;i++)key(g,d[i],(char)('1'+i),i%3,i/3);key(g,"",'<',0,3);key(g,"0",'0',1,3);key(g,"",'=',2,3);
}
void ui_loginScreen_screen_destroy(void){if(ui_loginScreen){lv_obj_del(ui_loginScreen);ui_loginScreen=NULL;}}