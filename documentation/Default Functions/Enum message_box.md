<style>
    .win-box{
        display: block;
        position: relative;
        left: 12.5%;
        width: 75%;
        height: 225px;
        border: 5px solid #07b3e3;
        margin-top: 2.5%;
        margin-bottom: 2.5%;
    }
    .win-box .win-title{
        display: flex;
        height: 25px;
        padding: 5px 5px;
        border-bottom: 5px solid #07b3e3;
        justify-content: space-between;
        align-items: flex-start
    }
    .win-box .win-buttons{
        gap: 5px;
    }
    .win-box .win-buttons button{
        width: 25px;
        height: 25px;
        padding: 0;
        padding-top: 1px;
        border: 5px solid #07b3e3;
        background-color: rgba(0,0,0,0);
        color: #07b3e3;
        cursor: pointer;
    }
    .win-box .win-content{
        display: flex;
        flex-direction: column;
        padding: 20px;
        justify-content: space-between;
        height: 66%;
    }
    .win-box .win-content .win-buttons{
        display: flex;
        justify-content: center;
        gap: 5px;
    }
    .win-box .win-content .win-buttons button{
        width: 33.33%;
        color: currentColor;
    }
</style>
# 🧾 message_box
### - ESP
Este enumerador indica cómo se debe formatear la ventana emergente producida por `message`.\
Cada etiqueta tiene aquí su vista previa (simplificaciones gráficas: la apariencia real varia con el sistema operativo, sus ajustes y temas).\
Los valores se pueden combinar entre sí mediante el operador binario OR (`|`) para construir configuraciones complejas.
### - ENG
This enum specifies how the pop-up window produced by `message` should be formatted.\
Each tag has its preview here (graphical simplifications: the real appearance varies with the operating system, its settings and themes).\
Values can be combined with each other using the bitwise OR operator (`|`) to build complex configurations.

## Buttons
### - ESP
Etiquetas encargadas de definir la cantidad, tipo y disposición de los botones inferiores que mostrará la ventana emergente.
### - ENG
Tags responsible for defining the number, label, and layout of the buttons displayed at the bottom of the pop-up window.
<table style="width:100%;">
    <tr>
        <th>Etiqueta del Enumerador / Enumerator Tag</th>
        <th>Valor / Value</th>
        <th style="text-align: right;width:50%;">Visual Example</th>
    </tr>
    <tr>
        <td><code>message_box::ok</code></td>
        <td><code>0x000</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>OK</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::okcancel</code></td>
        <td><code>0x001</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>OK</button><button>CANCEL</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::abortretryignore</code></td>
        <td><code>0x002</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>ABORT</button><button>RETRY</button><button>IGNORE</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::yesnocancel</code></td>
        <td><code>0x003</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>YES</button><button>NO</button><button>CANCEL</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::yesno</code></td>
        <td><code>0x004</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>YES</button><button>NO</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::retrycancel</code></td>
        <td><code>0x005</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>RETRY</button><button>CANCEL</button></div></div></div></td>
    </tr>
</table>

## Icons
### - ESP
Etiquetas que determinan el icono decorativo o informativo que acompañará al mensaje dentro del cuerpo de la ventana emergente.
### - ENG
Tags that determine the descriptive icon displayed alongside the text within the pop-up window body.
<table style="width:100%;">
    <tr>
        <th>Etiqueta del Enumerador / Enumerator Tag</th>
        <th>Valor / Value</th>
        <th style="text-align: right;width:50%;">Visual Example</th>
    </tr>
    <tr>
        <td><code>message_box::info</code></td>
        <td><code>0x000</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>OK</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::error</code></td>
        <td><code>0x010</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content"><div style="display:flex;"><div style="border:5px solid #e30707;background-color:black;border-radius:100%;width:26px;height:26px;margin-right:20px;text-align:center;font-size:25px;color:#e30707;"><p style="position:relative;top:-9px;">X</p></div>&lt;Text&gt;</div><div class="win-buttons"><button>OK</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::question</code></td>
        <td><code>0x020</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content"><div style="display:flex;"><div style="border:5px solid #07b3e3;;background-color:black;border-radius:100%;width:26px;height:26px;margin-right:20px;text-align:center;font-size:25px;color:#07b3e3;"><p style="position:relative;top:-9px;">?</p></div>&lt;Text&gt;</div><div class="win-buttons"><button>OK</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::warning</code></td>
        <td><code>0x030</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content"><div style="display:flex;"><div style="width:26px;height:26px;margin-right:29px;"><div style="position:absolute;width:0;height:0;border-left: 18px solid transparent;border-right: 18px solid transparent;border-bottom: 36px solid #a88b16;"></div><div style="position:absolute;top:66.6px;left:24px;width:0;height:0;border-left: 13px solid transparent;border-right: 13px solid transparent;border-bottom: 26px solid black;"></div><p style="position:absolute;top:59px;left:34px;color:#a88b16;font-size:25px;">&excl;</p></div>&lt;Text&gt;</div><div class="win-buttons"><button>OK</button></div></div></div></td>
    </tr>
</table>

## Default Buttons
### - ESP
Etiquetas que seleccionan cuál botón quedará como predeterminado al instanciarse la ventana.\
El botón por defecto es el que mantiene el foco inicial del teclado: se confirma con `Enter` sin mover el cursor con el ratón.\
Si la posición pedida excede la cantidad de botones creados, se usa el botón con la posición válida más alta (las posiciones van de izquierda a derecha, siendo la más alta la del botón más a la derecha).
### - ENG
Tags that select which button becomes the default when the pop-up is created.\
The default button holds the initial keyboard focus: it is confirmed with `Enter` without moving the mouse pointer.\
If the requested position exceeds the number of buttons created, the button with the highest valid position is used (positions run left to right, the highest being the rightmost button).
<table style="width:100%;">
    <tr>
        <th>Etiqueta del Enumerador / Enumerator Tag</th>
        <th>Valor / Value</th>
        <th>Combinación de Ejemplo / Example Combination</th>
        <th style="text-align: right;width:50%;">Visual Example</th>
    </tr>
    <tr>
        <td><code>message_box::defbutton1</code></td>
        <td><code>0x000</code></td>
        <td><code>message_box::defbutton1 | message_box::yesnocancel</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button style="background-color:#07b3e3;color:black;">YES</button><button>NO</button><button>CANCEL</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::defbutton2</code></td>
        <td><code>0x100</code></td>
        <td><code>message_box::defbutton2 | message_box::yesnocancel</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>YES</button><button style="background-color:#07b3e3;color:black;">NO</button><button>CANCEL</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::defbutton3</code></td>
        <td><code>0x200</code></td>
        <td><code>message_box::defbutton3 | message_box::yesnocancel</code></td>
        <td><div class="win-box"><div class="win-title"><p>&lt;Title&gt;</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">&lt;Text&gt;<div class="win-buttons"><button>YES</button><button>NO</button><button style="background-color:#07b3e3;color:black;">CANCEL</button></div></div></div></td>
    </tr>
</table>

## Examples
### - ESP
Combinaciones habituales y sus representaciones visuales esperadas.
### - ENG
Common combinations and their expected visual outputs.
<table style="width:100%;">
    <tr>
        <th>Combinación / Combination</th>
        <th>Valor Esperado / Expected Value</th>
        <th style="text-align: right;width:50%;">Visual Example</th>
    </tr>
    <tr>
        <td><code>message_box::yesno | message_box::warning | message_box::defbutton2</code></td>
        <td><code>0x134</code></td>
        <td><div class="win-box"><div class="win-title"><p>WARNING</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">  <div style="display:flex;"><div style="width:26px;height:26px;margin-right:29px;"><div style="position:absolute;width:0;height:0;border-left: 18px solid transparent;border-right: 18px solid transparent;border-bottom: 36px solid #a88b16;"></div><div style="position:absolute;top:66.6px;left:24px;width:0;height:0;border-left: 13px solid transparent;border-right: 13px solid transparent;border-bottom: 26px solid black;"></div><p style="position:absolute;top:59px;left:34px;color:#a88b16;font-size:25px;">&excl;</p></div><p style="width:75%;">This operation will permanently delete all files stored in the Recycle Bin.<br>Are you sure you want to continue?</p></div>  <div class="win-buttons"><button>YES</button><button style="background-color:#07b3e3;color:black;">NO</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::abortretryignore | message_box::error</code></td>
        <td><code>0x012</code></td>
        <td><div class="win-box"><div class="win-title"><p>ERROR</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">
          <div style="display:flex;"><div style="border:5px solid #e30707;background-color:black;border-radius:100%;width:26px;height:26px;margin-right:20px;text-align:center;font-size:25px;color:#e30707;"><p style="position:relative;top:-9px;">X</p></div><p style="width:75%;">Line: 273<br>An invalid memory access attempt was detected within the program.</p></div>  <div class="win-buttons"><button>ABORT</button><button>RETRY</button><button>IGNORE</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::ok | message_box::info</code></td>
        <td><code>0x000</code></td>
        <td><div class="win-box"><div class="win-title"><p>Weather</p><div class="win-buttons"><button>X</button></div></div><div class="win-content">The weather is cloudy, with an average temperature of 26°C, strong winds and hail starting at 9:13 PM.<div class="win-buttons"><button>OK</button></div></div></div></td>
    </tr>
    <tr>
        <td><code>message_box::retrycancel | message_box::question</code></td>
        <td><code>0x025</code></td>
        <td><div class="win-box"><div class="win-title"><p>Bluetooth Device</p><div class="win-buttons"><button>X</button></div></div><div class="win-content"><div style="display:flex;"><div style="border:5px solid #07b3e3;;background-color:black;border-radius:100%;width:26px;height:26px;margin-right:20px;text-align:center;font-size:25px;color:#07b3e3;"><p style="position:relative;top:-9px;">?</p></div><p style="width:75%;">No Bluetooth device has been detected with which to make a connection.<br>The file you tried to transfer is still in its original folder.</p></div><div class="win-buttons"><button>RETRY</button><button>CANCEL</button></div></div></div></td>
    </tr>
</table>

## Results
### - ESP
Las etiquetas de `message_box` describen la **apariencia** de la ventana.\
El entero que devuelve `message` describe la **respuesta** del usuario: son dos espacios de valores distintos y nunca deben compararse entre sí.\
El valor devuelto sigue la tabla siguiente, con la misma numeración en ambas plataformas (la rama de Linux traduce los códigos de salida de `zenity` a los equivalentes de la API de Windows).\
Cerrar la ventana desde el botón `X` se reporta como `Cancelar`.\
Excepción: en el esquema `message_box::abortretryignore`, la `X` se reporta como `Abortar`.
| Valor | Respuesta | Botón que lo produce |
| :---- | :---- | :---- |
| `1` | Aceptar | OK |
| `2` | Cancelar | Cancelar, o la `X` de la ventana |
| `3` | Abortar | Abortar, o la `X` con `message_box::abortretryignore` |
| `4` | Reintentar | Retry |
| `5` | Ignorar | Ignore |
| `6` | Sí | Yes |
| `7` | No | No |
Un valor `0` o negativo no es una respuesta del usuario, sino un fallo de la propia función.\
`-1`: la función de sistema que muestra la ventana no pudo resolverse (en Linux suele significar que `zenity` no está instalado).\
`-2`: `Txt` o `Ttl` fueron nulos.\
Por eso se compara el resultado contra un valor concreto de esta tabla, nunca como booleano.
### - ENG
The `message_box` tags describe the dialog's **appearance**.\
The integer returned by `message` describes the user's **response**: these are two distinct value spaces and must never be compared against each other.\
The returned value follows the table below, numbered the same on both platforms (the Linux branch translates `zenity` exit codes into their Windows API equivalents).\
Closing the window with its `X` button reports `Cancel`.\
Exception: in the `message_box::abortretryignore` layout, the `X` reports `Abort`.
| Value | Response | Button producing it |
| :---- | :---- | :---- |
| `1` | OK | OK |
| `2` | Cancel | Cancel, or the window's `X` |
| `3` | Abort | Abort, or the `X` while using `message_box::abortretryignore` |
| `4` | Retry | Retry |
| `5` | Ignore | Ignore |
| `6` | Yes | Yes |
| `7` | No | No |
A value of `0` or below is not a user response but a failure of the call itself.\
`-1`: the system function that shows the dialog could not be resolved (on Linux this usually means `zenity` is not installed).\
`-2`: `Txt` or `Ttl` was null.\
The result is therefore compared against a specific value from this table, never used as a boolean.